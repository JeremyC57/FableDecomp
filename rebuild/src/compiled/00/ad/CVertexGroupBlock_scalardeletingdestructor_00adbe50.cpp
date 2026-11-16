// CVertexGroupBlock::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor of a non-polymorphic class:
// ~CVertexGroupBlock (0x00A96DA0), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCVertexGroupBlock@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADBE50, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// NF39 (2026-11-16): moved off its CLandscapeBackgroundPatch `vector deleting destructor` stand-in (a propagated label) onto the shared
//   engine/CVertexGroupBlock.h. Identity: the only callee is 0x00A96DA0 = ??1CVertexGroupBlock@@QAE@XZ (Ego_r.pdb + FableWin.pdb); FableWin.pdb's name
//   set holds ??_GCVertexGroupBlock@@QAEPAXI@Z; a non-virtual class's `delete p` emits the Q-access ??_G as its own COMDAT (landed G042 family, 0x0075F0B0).
// Callees: CVertexGroupBlock::~CVertexGroupBlock() 0x00A96DA0, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

void Kill_00adbe50(CVertexGroupBlock* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
