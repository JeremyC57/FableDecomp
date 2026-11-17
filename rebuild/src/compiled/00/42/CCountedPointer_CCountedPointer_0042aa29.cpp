// CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction* data) -- adopt the action: a null-checked count block {1, DeleteData, data}
// Leaf: ??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AA29, 62 bytes (`ret 4`): Data stored, then for a non-null object `new(0Ch)` (0x00BFEA1A,
//   null-checked) filled {1, DeleteData 0x00429F43, Data re-read from [esi]}, else Info = 0. Size TU (#pragma optimize("s", on)).
// Identity: engine/CCountedPointer.h (Ego_r.pdb CCountedPointer<T>, a global template; FableWin.pdb ??0?$CCountedPointer@..@@QAE@PAV..@Z
//   instances) instantiated for NUISystem::CAction, whose per-type deleter 0x00429F43 is ?DeleteData@?$CCountedPointer@VCAction@NUISystem@@@@SIXPAX@Z
//   (Ego_r.pdb + FableWin.pdb, NF37); the landed callers 0x00549B20 (NF76) and 0x0054B4B0 bind this spelling.
// NF79 (round 7, q10964): re-authored as the explicit instantiation of the header's (T*) constructor (FABLE_RETAIL_CCOUNTEDPOINTER: the
//   test of the stored member), the form of the landed CCountedPointer<CCreatureActionBase> ctor 0x00695880; was the stand-in struct
//   CCountedPointer_00429f43 with a local count block.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "engine/CCountedPointer.h"
namespace NUISystem { class CAction; }
template CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction*);
