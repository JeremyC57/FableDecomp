// CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bace10, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A5430 (RTTI .?AVCEngineSubPrimitiveSprite@@ : CEngineSubPrimitiveBase); FableWin.pdb ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z; no direct callers found (unbound).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family L116 = landed family K056, 0x00BB1780): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb Parent +0x0C,
//   NextSubPrimitive +0x10 cleared); the base's own vptr store is dead and dropped, leaving only 0x012A5430.
// CEngineSubPrimitiveSprite : CEngineSubPrimitiveBase : CBaseClassNonCopyable -- the shared headers; the constructor is declared
// (header opt-in) and defined out of line here; the destructors are implicit, as in the row's earlier model and the sibling sub-primitive
// rows (the vtable's ??_G calls ~CBaseClassNonCopyable 0x0099A300 directly, 0x00BB1740).
#define FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVESPRITE_CTOR_OUTOFLINE
#include "engine/CEngineSubPrimitiveSprite.h"

CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent)
    : CEngineSubPrimitiveBase(parent)
{
}