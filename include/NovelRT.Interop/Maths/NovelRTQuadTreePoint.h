// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.
#include "NovelRT.Interop/Maths/NovelRTGeoVector2F.h"

#ifndef NOVELRT_INTEROP_MATHS_QUADTREEPOINT_H
#define NOVELRT_INTEROP_MATHS_QUADTREEPOINT_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct QuadtreePoint* NovelRTQuadTreePoint;
  
  NovelRTQuadTreePoint NovelRT_QuadTreePoint_create(NovelRTGeoVector2F position);
  NovelRTQuadTreePoint NovelRT_QuadTreePoint_createFromFloat(float x, float y);
  NovelRTGeoVector2F NovelRT_QuadTreePoint_getPosition(const NovelRTQuadTreePoint const point);
  NovelRTResult NovelRT_QuadTreePoint_delete(NovelRTQuadTreePoint point);

#ifdef __cplusplus
}
#endif

#endif //NOVELRT_INTEROP_MATHS_QUADTREEPOINT_H