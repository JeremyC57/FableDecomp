// CWorldMap::DrawIsPosInMap(const C3DVector&) const -- draw interface: is the world position inside a map (IsPosInMap)
// Leaf: ?DrawIsPosInMap@CWorldMap@@UBE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FC6B0, 8 bytes: `add ecx,-4 ; jmp 0x004FC600`.
// Identity: CIDrawWorldMap slot 4 of CWorldMap's secondary vtable 0x01244AB4 (rtti-vtables.tsv); Ego_r.pdb / FableWin.pdb
//   ?DrawIsPosInMap@CWorldMap@@UBE_NABVC3DVector@@@Z (Ego_r 0x004CFE60, right after IsPosInMap(const C3DVector&) 0x004CFDB0,
//   as retail 0x004FC6B0 follows IsPosInMap 0x004FC600).
// Callee: 0x004FC600 = ?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z (both PDBs; engine/CWorldMap.h retail branch).
// Shape: not a compiler adjustor thunk (`sub ecx,4`): the override is only in the CIDrawWorldMap vtable, so VC7.1 compiles it
//   with `this` at the +4 interface and converts back to the CWorldMap with `add ecx,-4` before the tail jump.
// Layout: engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP: CBaseClassNonCopyable +0x00, CIDrawWorldMap +0x04).
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorldMap.h"

bool CWorldMap::DrawIsPosInMap(const C3DVector& pos) const
{
    return IsPosInMap(pos);
}
