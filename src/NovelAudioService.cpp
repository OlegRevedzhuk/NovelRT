// Copyright © Matt Jones and Contributors. Licensed under the MIT License (MIT). See LICENCE.md in the repository root for more information.

#include "NovelAudioService.h"
#include "NovelRTUtilities.h"
#include <iostream>
#include <functional>
#include <stdexcept>

namespace NovelRT {

NovelAudioService::NovelAudioService() : _nextChannel(1), _musicTime(0), _musicPausedTime(0), isInitialized(false), _logger(NovelUtilities::CONSOLE_LOG_AUDIO) {
  initializeAudio();
}

bool NovelAudioService::initializeAudio() {
  logIfSDLFailure(SDL_InitSubSystem, SDL_INIT_AUDIO);
  logIfMixerFailure(Mix_OpenAudio, 44100, MIX_DEFAULT_FORMAT, 2, 2048);
  logIfMixerFailure(Mix_AllocateChannels, (Uint32)NOVEL_MIXER_CHANNELS);
  _logger.logInfoLine("SDL2_Mixer Initialized.");
  isInitialized = true;
  return isInitialized;
}

void NovelAudioService::load(std::string input, bool isMusic) {
  if (!isMusic)
  {
    auto exists = _sounds.find(input);
    if (exists != _sounds.end()) return;

    Mix_Chunk* newSound = Mix_LoadWAV(input.c_str());
    if (newSound != nullptr)
    {
      _sounds[input] = newSound;
    }
    else
    {
      //_logger.logError("SDL_Mixer error occurred during load. Error: ", getSDLError());
    }
  }
  else
  {
    auto exists = _music.find(input);
    if (exists != _music.end()) return;

    Mix_Music* newMusic = Mix_LoadMUS(input.c_str());
    if (newMusic != nullptr)
    {
      _music[input] = newMusic;
    }
    else
    {
     // _logger.logError("SDL_Mixer error occurred during load. Error: ", getSDLError());
    }
  }
}

void NovelAudioService::unload(std::string input, bool isMusic) {
  if (!isMusic)
  {
    auto existingSound = _sounds.find(input);
    if (existingSound == _sounds.end()) return;

    Mix_FreeChunk(_sounds[input]);
    _sounds.erase(existingSound);
  }
  else
  {
    auto existingMusic = _music.find(input);
    if (existingMusic == _music.end()) return;

    Mix_FreeMusic(_music[input]);
    _music.erase(existingMusic);
  }
}

void NovelAudioService::playSound(std::string soundName, int loops) {
  auto existingSound = _sounds.find(soundName);
  if (existingSound == _sounds.end())
  {
    load(soundName, false);
    existingSound = _sounds.find(soundName);
    if (existingSound == _sounds.end())
    {
      return;
    }
  }

  auto onChannel = _channelMap.find(soundName);
  if (onChannel != _channelMap.end())
  {
    if (Mix_Playing(_channelMap[soundName]) == MIXER_TRUE)
    {
      _logger.logWarning("Already playing on channel " + _channelMap[soundName]);
      return;
    }

    if (loops == MIXER_INFINITE_LOOP)
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], MIXER_INFINITE_LOOP);
    }
    else if (loops == MIXER_NO_LOOP)
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], MIXER_NO_LOOP);
    }
    else
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], loops-1);
    }
  }
  else
  {
    _channelMap[soundName] = _nextChannel;
    incrementNextChannel();

    if (loops == MIXER_INFINITE_LOOP)
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], MIXER_INFINITE_LOOP);
    }
    else if (loops == MIXER_NO_LOOP)
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], MIXER_NO_LOOP);
    }
    else
    {
      Mix_PlayChannel(_channelMap[soundName], _sounds[soundName], loops-1);
    }
  }
}

void NovelAudioService::stopSound(std::string soundName) {
  Mix_HaltChannel(_channelMap[soundName]);
}

void NovelAudioService::setSoundVolume(std::string soundName, float value) {
  Mix_VolumeChunk(_sounds[soundName], convertToMixVolume(value));
}

void NovelAudioService::setSoundPosition(std::string soundName, int angle, int distance) {
  if (_channelMap.find(soundName) != _channelMap.end() && _channelMap[soundName] != MIXER_NO_EXPLICIT_CHANNEL)
  {
    Mix_SetPosition(_channelMap[soundName], angle, distance);
  }
  else
  {
    _logger.logErrorLine(soundName + " is not allocated to a channel already!");
  }
}

void NovelAudioService::setSoundDistance(std::string soundName, int distance) {
    if (_channelMap.find(soundName) != _channelMap.end() && _channelMap[soundName] != MIXER_NO_EXPLICIT_CHANNEL)
  {
    Mix_SetDistance(_channelMap[soundName], distance);
  }
  else
  {
    _logger.logErrorLine(soundName + " is not allocated to a channel already!");
  }
}

void NovelAudioService::setSoundPanning(std::string soundName, int leftChannelVolume, int rightChannelVolume) {
    if (_channelMap.find(soundName) != _channelMap.end() && _channelMap[soundName] != MIXER_NO_EXPLICIT_CHANNEL)
  {
    Mix_SetPanning(_channelMap[soundName], leftChannelVolume, rightChannelVolume);
  }
  else
  {
    _logger.logErrorLine(soundName + " is not allocated to a channel already!");
  }
}

void NovelAudioService::resumeMusic() {
  if (Mix_PausedMusic()) Mix_ResumeMusic();
}

void NovelAudioService::playMusic(std::string musicName, int loops) {

  auto existingMusic = _music.find(musicName);
  if (existingMusic == _music.end())
  {
    load(musicName, true);
    existingMusic = _music.find(musicName);
    if (existingMusic == _music.end()) return;
  }
  _channelMap[musicName] = NOVEL_MUSIC_CHANNEL;

  if (loops == 0)
  {
    Mix_PlayMusic(_music[musicName], MIXER_NO_LOOP);
  }
  else if (loops == MIXER_INFINITE_LOOP)
  {
    Mix_PlayMusic(_music[musicName], MIXER_INFINITE_LOOP);
  }
  else
  {
    Mix_PlayMusic(_music[musicName], loops-1);
  }


}

void NovelAudioService::pauseMusic() {
  if (!Mix_PausedMusic())
  {
    Mix_PauseMusic();
    _musicPausedTime = SDL_GetPerformanceCounter();
  }
}

void NovelAudioService::stopMusic() {
  Mix_HaltMusic();
}

void NovelAudioService::setMusicVolume(float value) {
  Mix_VolumeMusic(convertToMixVolume(value));
}

void NovelAudioService::fadeMusicInOnce(std::string musicName, int ms) {
  if (!Mix_PlayingMusic())
  {
    Mix_FadeInMusic(_music[musicName], MIXER_NO_LOOP, ms);
  }
  else
  {
    Mix_FadeOutMusic(1000);
    switch(Mix_GetMusicType(_music[musicName]))
    {
      case MUS_OGG:
      {
        double pos = (double)((_musicPausedTime - _musicTime)*1000) / (double)SDL_GetPerformanceFrequency();
        Mix_SetMusicPosition(pos);
        Mix_FadeInMusic(_music[musicName], MIXER_NO_LOOP, ms);
        break;
      }
      case MUS_MP3:
      {
        Mix_RewindMusic();
        double pos = (double)((_musicPausedTime - _musicTime)*1000) / (double)SDL_GetPerformanceFrequency();
        Mix_SetMusicPosition(pos);
        Mix_FadeInMusic(_music[musicName], MIXER_NO_LOOP, ms);
        break;
      }
      default:
      {
        Mix_FadeInMusic(_music[musicName], MIXER_NO_LOOP, ms);
        break;
      }
    }
  }
}

void NovelAudioService::fadeMusicIn(std::string musicName, int loops, int ms) {

  if (!Mix_PlayingMusic())
  {
    Mix_FadeInMusic(_music[musicName], loops-1, ms);
  }
  else
  {
    Mix_FadeOutMusic(1000);
    switch(Mix_GetMusicType(_music[musicName]))
    {
      case MUS_OGG:
      {
        double pos = (double)((_musicPausedTime - _musicTime)*1000) / (double)SDL_GetPerformanceFrequency();
        Mix_SetMusicPosition(pos);
        Mix_FadeInMusic(_music[musicName], loops-1, ms);
        break;
      }
      case MUS_MP3:
      {
        Mix_RewindMusic();
        double pos = (double)((_musicPausedTime - _musicTime)*1000) / (double)SDL_GetPerformanceFrequency();
        Mix_SetMusicPosition(pos);
        Mix_FadeInMusic(_music[musicName], loops-1, ms);
        break;
      }
      default:
      {
        Mix_FadeInMusic(_music[musicName], loops-1, ms);
        break;
      }
    }
  }
}

void NovelAudioService::fadeMusicOut(int ms) {
  Mix_FadeOutMusic(ms);
  _musicPausedTime = SDL_GetPerformanceCounter();
}

void NovelAudioService::setGlobalVolume(float value) {
  Mix_Volume(MIXER_NO_EXPLICIT_CHANNEL, convertToMixVolume(value));
  Mix_VolumeMusic(convertToMixVolume(value));
}

int NovelAudioService::convertToMixVolume(float value) {
  int converted = (value > 1.0f || value < 0.0f) ? 1.0f : (int)(SDL_MIX_MAXVOLUME * value);
  return converted;
}

void NovelAudioService::incrementNextChannel() {
  int nextChannelTest = _nextChannel + 1;
  _nextChannel = (nextChannelTest >= NOVEL_MIXER_CHANNELS || nextChannelTest < 0) ? 0 : nextChannelTest;
}

std::string NovelAudioService::findByChannelMap(int channel) {
  auto it = _channelMap.begin();
  while (it != _channelMap.end())
  {
    if (it->second == channel)
    {
      return it->first;
    }
    it++;
  }

  return nullptr;
}

void NovelAudioService::logIfSDLFailure(int (*function)(Uint32), Uint32 sdl_flag) {
  if (function(sdl_flag) < NovelUtilities::SDL_SUCCESS)
  {
    _logger.logError("SDL Error: ", getSDLError());
    throw std::runtime_error("Audio error occurred! Unable to continue.");
  }
}

void NovelAudioService::logIfMixerFailure(int (*function)(int), int mixerFlag) {
  if (function(mixerFlag) < NovelUtilities::SDL_SUCCESS)
  {
    _logger.logError("SDL Error: ", getSDLError());
    throw std::runtime_error("Audio error occurred! Unable to continue.");
  }
}

void NovelAudioService::logIfMixerFailure(int (*function)(int, Uint16, int, int), int freq, Uint16 mixerFormat, int channels, int sampleSize) {
  if (function(freq, mixerFormat, channels, sampleSize) < NovelUtilities::SDL_SUCCESS)
  {
    _logger.logError("SDL Error: ", getSDLError());
    throw std::runtime_error("Audio error occurred! Unable to continue.");
  }
}

std::string NovelAudioService::getSDLError() {
  return std::string(SDL_GetError());
}

NovelAudioService::~NovelAudioService() {
  Mix_HaltMusic();
  Mix_HaltChannel(MIXER_NO_EXPLICIT_CHANNEL);

  std::map<std::string, Mix_Music*>::iterator musicIterator;
  for (musicIterator = _music.begin(); musicIterator != _music.end(); musicIterator++)
  {
    Mix_FreeMusic(musicIterator->second);
  }

  std::map<std::string, Mix_Chunk*>::iterator soundIterator;
  for (soundIterator = _sounds.begin(); soundIterator != _sounds.end(); soundIterator++)
  {
    Mix_FreeChunk(soundIterator->second);
  }
  _channelMap.clear();
  _music.clear();
  _sounds.clear();

  Mix_Quit();
}

}
