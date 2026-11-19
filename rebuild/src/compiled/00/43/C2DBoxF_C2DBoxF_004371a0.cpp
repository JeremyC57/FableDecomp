// C2DBoxF::C2DBoxF(const C2DVector& tl, const C2DVector& br) -- box from its two corners: copy each corner's X / Y
// Leaf: ??0C2DBoxF@@QAE@ABVC2DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004371A0, 34 bytes, thiscall `ret 8` (`this` returned in eax; four plain dword copies).
// NF104 (q4918, 2026-11-19): moved off its `CRightHandedSet(RHSHolder*, int, const Pair*, const Pair*)` free __fastcall stand-in (a propagated
//   label) onto the PDB spelling: Ego_r.pdb + FableWin.pdb ??0C2DBoxF@@QAE@ABVC2DVector@@0@Z (both name sets list only the reference pair, no
//   pointer overload); CEnginePrimitive2DSprite::CEnginePrimitive2DSprite 0x00493AC2 calls it with the position twice and binds this symbol.
//   The body is the shared engine/C2DBoxF.h inline corners constructor (FABLE_RETAIL_C2DBOXF + _CORNERS; size TUs call it out of line, as a
//   pick-any COMDAT), emitted here by a scaffolding reference rather than a second, strong definition.
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#include <new>
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"

namespace {
// Fidelity: scaffolding, not retail: a placement construction through the header's inline corners constructor with inline expansion switched
// off for this one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x004371A0). Internal; never called.
#pragma inline_depth(0)
void ReferenceCornersCtor(void* where, const C2DVector& tl, const C2DVector& br)
{
    new (where) C2DBoxF(tl, br);
}
#pragma inline_depth()
}
