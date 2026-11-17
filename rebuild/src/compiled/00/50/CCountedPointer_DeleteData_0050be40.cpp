// CCountedPointer<CAFile>::DeleteData(void* data) -- the per-type deleter stored in the count block: delete the file through its virtual
//   destructor (`test ecx,ecx ; je ; mov eax,[ecx] ; push 1 ; call [eax]`).
// Leaf: ?DeleteData@?$CCountedPointer@VCAFile@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050BE40, 11 bytes.
// Identity (NF56, round 7, queue q10465): naming wave 1 decision (rebuild/candidates/manual/naming/2026-10-28_wave1/DECISIONS.tsv line 131,
//   FableWin.pdb 0x01CB0BA0 ?DeleteData@?$CCountedPointer@VCAFile@@@@SIXPAX@Z); the landed CWorldMap::LoadFromFile 0x00507C30 and
//   GetLevelStaticMapChecksum 0x00506C40 store it through engine/CCountedPointer.h CCountedPointer<CAFile> and bind this spelling. The body
//   is the shared polymorphic deleter shape, so other CCountedPointer<T> deleters fold onto this address (ICF); the self row takes the decided
//   T. CAFile is polymorphic (engine/CAFile.h : CBaseClassNonCopyable, virtual destructor slot 0). Was the stand-in free DeleteData over a
//   local AutoTinyVirtualObject (catalog _global::DeleteViaVtable0_0050be40).
#include "engine/CAFile.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CAFile>::DeleteData(void*);
