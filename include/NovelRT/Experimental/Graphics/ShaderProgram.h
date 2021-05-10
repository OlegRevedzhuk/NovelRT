// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root
// for more information.

#ifndef NOVELRT_EXPERIMENTAL_SHADERPROGRAM_H
#define NOVELRT_EXPERIMENTAL_SHADERPROGRAM_H

#ifndef NOVELRT_EXPERIMENTAL_GRAPHICS_H
#error NovelRT does not support including types explicitly by default. Please include Graphics.h instead for the Graphics namespace subset.
#endif

namespace NovelRT::Experimental::Graphics
{
    class ShaderProgram : public GraphicsDeviceObject
    {
    private:
        std::string _entryPointName;
        ShaderProgramKind _kind;

    public:
        ShaderProgram(std::shared_ptr<ILLGraphicsDevice> device, std::string entryPointName, ShaderProgramKind kind) noexcept;

        [[nodiscard]] inline const std::string& EntryPointName() const noexcept
        {
            return _entryPointName;
        }

        [[nodiscard]] inline ShaderProgramKind Kind() const noexcept
        {
            return _kind;
        }

        [[nodiscard]] virtual gsl::span<uint8_t> GetBytecode() const noexcept = 0;
    };
} // namespace NovelRT::Experimental::Graphics

#endif // !NOVELRT_EXPERIMENTAL_SHADERPROGRAM_H