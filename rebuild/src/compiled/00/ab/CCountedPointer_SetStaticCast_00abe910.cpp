// CCountedPointer::SetStaticCast(const CCountedPointer<U>& other) -- share another counted pointer's object and reference block (static cast, zero adjustment): release the current block unless it is the same one, take the other's data and info, add a reference
// Leaf: ??$SetStaticCast@VCUnprovenSource_00abe910@@@?$CCountedPointer@VCUnprovenTarget_00abe910@@@@QAEXABV?$CCountedPointer@VCUnprovenSource_00abe910@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABE910, 70 bytes, thiscall `ret 4`, no result.
// Identity (probable, descriptive types): a CCountedPointer<T> member taking `const CCountedPointer<U>&` and returning
//   nothing is, in the PDB name sets, only the member template SetStaticCast<U> (74 instantiations, almost all
//   CCountedPointer<C3DMeshFile*Chunk / C3DAnimFile*Chunk>::SetStaticCast<CChunkedFileChunk>, the mesh/anim chunk readers
//   this address sits among). No call or pointer reaches 0x00ABE910 (the "abs" hits 0x00437E6C / 0x00496BB7 /
//   0x0061A784 are bytes inside other instructions), so T and U are unproven: CUnprovenTarget_00abe910 /
//   CUnprovenSource_00abe910 stand in (T derives from U at offset 0, so the static cast moves the pointer unchanged).
// Callee: operator delete @ 0x00BFE9BC (the reference block). The block's delete function is called through +4 with
//   the object (+8) in ecx (__fastcall).
// Layout: CCountedPointer<T> (8 bytes): Data +0, Info +4 (CCPPointerInfo: RefCount +0, DeleteFunc +4, Data +8).
// Shape: SetStaticCast forwards to the inline ShareData(static_cast<T*>(other.Data), other.Info) (the landed
//   CCountedPointer<CScriptThing>::ShareData 0x00A6E1D0 body): both source words are loaded first, the block re-read after
//   the decrement, the new block tested from the argument register.
#include "engine/CCountedPointer.h"


class CUnprovenSource_00abe910 { };
class CUnprovenTarget_00abe910 : public CUnprovenSource_00abe910 { };

template void CCountedPointer<CUnprovenTarget_00abe910>::SetStaticCast<CUnprovenSource_00abe910>(const CCountedPointer<CUnprovenSource_00abe910>& other);