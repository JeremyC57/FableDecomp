// CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bace10, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A5430 (RTTI .?AVCEngineSubPrimitiveSprite@@ : CEngineSubPrimitiveBase); FableWin.pdb ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z; no direct callers found (unbound).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family L116 = landed family K056, 0x00BB1780): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb Parent +0x0C,
//   NextSubPrimitive +0x10 cleared); the base's own vptr store is dead and dropped, leaving only 0x012A5430.
// CEngineSubPrimitiveSprite : CEngineSubPrimitiveBase : CBaseClassNonCopyable -- the shared headers; the destructors are implicit, as in
// the row's earlier model and the sibling sub-primitive rows (the vtable's ??_G calls ~CBaseClassNonCopyable 0x0099A300 directly, 0x00BB1740).
// One definition (2026-11-20, B reports_b4 follow-up; advisor decision, owner delegated): this row defined the constructor strongly out of
// line (FABLE_RETAIL_CENGINESUBPRIMITIVESPRITE_CTOR_OUTOFLINE) while the header's includers (0x00BADE40, B's 0x00BB1740 ??_G rewrite)
// emit its inline body pick-any: LNK2005. It now emits the ONE header body as a pick-any COMDAT through the nf86 / nf88 scaffolding below.
#define FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
#include "engine/CEngineSubPrimitiveSprite.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline constructor, with inline expansion switched off
// for this one helper, so this object emits it as a pick-any COMDAT. Internal to this TU; nothing calls it.
#pragma inline_depth(0)
void ReferenceSubPrimitiveSpriteCtor(CEngineInternalPrimitiveSprite* parent)
{
    CEngineSubPrimitiveSprite sub_primitive(parent);
}
#pragma inline_depth()
}
