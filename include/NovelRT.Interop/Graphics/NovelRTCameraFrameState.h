// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#ifndef NOVELRT_INTEROP_GRAPHICS_CAMERAFRAMESTATE_H
#define NOVELRT_INTEROP_GRAPHICS_CAMERAFRAMESTATE_H

#include "NovelRTGraphicsTypedefs.h"
#include "../Maths/NovelRTGeoMatrix4x4F.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  Unmodified = 0,
  ModifiedInCurrent = 1,
  ModifiedInLast = 2
} NovelRTCameraFrameState;

#ifdef __cplusplus
}
#endif

#endif //!NOVELRT_INTEROP_GRAPHICS_CAMERAFRAMESTATE_H