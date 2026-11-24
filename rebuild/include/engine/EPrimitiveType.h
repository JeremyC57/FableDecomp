#ifndef FABLE_ENGINE_EPRIMITIVETYPE_H
#define FABLE_ENGINE_EPRIMITIVETYPE_H
enum EPrimitiveType { PT_NULL=-1,PT_TRIANGLE_LIST=4,PT_TRIANGLE_STRIP=5,PT_TRIANGLE_FAN=6,PT_LINE_LIST=2,PT_LINE_STRIP=3,PT_POINT_LIST=1 };

#ifdef FABLE_RETAIL_ORIGINAL_PRIMITIVE_TO_D3D_PASS26
#include "fable_compiler.h"
#include "fable_assert.h"

#include "engine/Direct3D9Types.h"


// FableWin 02DD0060 checks PT_NULL before returning the enum conversion.
inline D3DPRIMITIVETYPE FABLE_FASTCALL GFPrimitiveTypeToD3DPT(EPrimitiveType prim_type)
{
    FABLE_RETAIL_ASSERT(prim_type!=PT_NULL);
    return static_cast<D3DPRIMITIVETYPE>(prim_type);
}
#endif // FABLE_RETAIL_ORIGINAL_PRIMITIVE_TO_D3D_PASS26
#endif
