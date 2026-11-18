// CCountedPointer<CCreatureActionBase>::Reset(CCreatureActionBase* data) -- drop the shared count block (destroying the action on the last
// reference), then adopt the new action with a fresh count block (count 1, the per-type deleter DeleteData 0x00668610)
// Leaf: ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00668D90, 103 bytes, thiscall `ret 4`.
// Identity (NF83, round 7, q11020 self): Ego_r.pdb / FableWin.pdb keep CCountedPointer<T>::Reset(T*) out of line; this instance's deleter
//   0x00668610 is ?DeleteData@?$CCountedPointer@VCCreatureActionBase@@@@SIXPAX@Z (NF37, both PDBs), and the landed
//   0x00713140 (NF79) binds this address as ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z.
//   Was the stand-in CCountedPointer_00668610::Reset(void*) over a local SCountBlock model.
// Port (NF83): one explicit instantiation of the shared engine/CCountedPointer.h Reset (the 0x009EE540 recipe).
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
template void CCountedPointer<CCreatureActionBase>::Reset(CCreatureActionBase*);
