#ifndef FABLE_ENGINE_NVECTORMATH_H
#define FABLE_ENGINE_NVECTORMATH_H
// NEW (chunk 2008, row 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals): the NVectorMath free functions that row calls, Ego_r.pdb /
// FableWin.pdb spellings (__cdecl: the callers clean the stack, `add esp,0Ch` / `add esp,14h`), and the SSE flag GFInitVectorMath sets.
// Declarations only (the landed rows declared them locally).
#include "rebuild_abi.h"
class C4DVector;
namespace NVectorMath {
void GFClear4DVectors(C4DVector* vectors, unsigned long count, const C4DVector& value);                 // 0x00A5C300 ?GFClear4DVectors@NVectorMath@@YAXPAVC4DVector@@KABV2@@Z
void GFNormalise3DVectors(const C4DVector* source, unsigned long source_stride, C4DVector* destination,
                          unsigned long destination_stride, unsigned long count);                       // 0x00A5C360 ?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z
}
extern bool g_SSEAvailable;                                              // 0x013D2880 ?g_SSEAvailable@@3_NA (established spelling)
#endif
