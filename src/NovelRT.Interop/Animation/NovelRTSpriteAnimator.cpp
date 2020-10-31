// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#include "../NovelRTInteropErrorHandlingInternal.h"
#include "NovelRT.Interop/Animation/NovelRTAnimatorPlayState.h"
#include "NovelRT.Interop/Animation/NovelRTSpriteAnimatorState.h"
#include "NovelRT.Interop/NovelRTNovelRunner.h"
#include "NovelRT.Interop/Graphics/NovelRTGraphicsTypedefs.h"
#include "NovelRT.Interop/Animation/NovelRTSpriteAnimator.h"
#include <NovelRT.h>

#ifdef __cplusplus
using namespace NovelRT;
extern "C" {
#endif

int32_t NovelRT_SpriteAnimator_create(NovelRTNovelRunner runner, NovelRTImageRect rect, NovelRTSpriteAnimator* outputAnimator) {
    if (runner == nullptr || rect == nullptr || outputAnimator == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* animator = new Animation::SpriteAnimator(reinterpret_cast<NovelRunner*>(runner), reinterpret_cast<Graphics::ImageRect*>(rect));
    *outputAnimator = reinterpret_cast<NovelRTSpriteAnimator>(animator);
    return NOVELRT_SUCCESS;
}

int32_t NovelRT_SpriteAnimator_play(NovelRTSpriteAnimator animator) {
    if (animator == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* cppAnimator = reinterpret_cast<Animation::SpriteAnimator*>(animator);
    cppAnimator->play();
    return NOVELRT_SUCCESS;
}

int32_t NovelRT_SpriteAnimator_pause(NovelRTSpriteAnimator animator) {
    if (animator == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* cppAnimator = reinterpret_cast<Animation::SpriteAnimator*>(animator);
    cppAnimator->pause();
    return NOVELRT_SUCCESS;
}

int32_t NovelRT_SpriteAnimator_stop(NovelRTSpriteAnimator animator) {
    if (animator == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* cppAnimator = reinterpret_cast<Animation::SpriteAnimator*>(animator);
    cppAnimator->stop();
    return NOVELRT_SUCCESS;
}

int32_t NovelRT_SpriteAnimator_getCurrentPlayState(NovelRTSpriteAnimator animator, NovelRTAnimatorPlayState* outputPlayState) {
    if (animator == nullptr || outputPlayState == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* cppAnimator = reinterpret_cast<Animation::SpriteAnimator*>(animator);
    Animation::AnimatorPlayState state = cppAnimator->getCurrentPlayState();
    *outputPlayState = reinterpret_cast<NovelRTAnimatorPlayState&>(state);
    return NOVELRT_SUCCESS;
}

int32_t NovelRT_SpriteAnimator_insertNewState(NovelRTSpriteAnimator animator, NovelRTSpriteAnimatorState state) {
    if (animator == nullptr || state == nullptr) {
        NovelRT_setErrMsgIsNullptrInternal();
        return NOVELRT_FAILURE;
    }

    Animation::SpriteAnimator* cppAnimator = reinterpret_cast<Animation::SpriteAnimator*>(animator);
    std::shared_ptr<Animation::SpriteAnimatorState> cppState = std::shared_ptr<Animation::SpriteAnimatorState>(reinterpret_cast<Animation::SpriteAnimatorState*>(state));
    cppAnimator->insertNewState(cppState);
    return NOVELRT_SUCCESS;
}

#ifdef __cplusplus
}
#endif