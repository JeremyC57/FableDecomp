// CCountedPointer::Reset(T* object) -- release the old share (out-of-line ReleaseData) then adopt a new raw object
// Leaf: ?Reset@?$CCountedPointer@VCAction@NUISystem@@@@QAEXPAVCAction@NUISystem@@@Z
// Retail: 005791e5, 59 bytes. PDB Reset/DeleteData, destructor-feed and callback targets prove NUISystem::CAction.
// Binder respelling (agentFAM13, FAM12 notes): the release call targets 0x00429160, which is
//   ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ (FableWin.pdb; the destructor is the 5-byte jmp COMDAT 0x00429B72),
//   not ~CCountedPointer<NUISystem::CAction> as the local model spelled it. Ported onto the shared engine/CCountedPointer.h
//   (Reset = ReleaseData(); Data = data; if (data) Info = new CCPPointerInfo(1, DeleteData, Data)) with the per-TU opt-in
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (ReleaseData out of line), replacing the local CCountedPointer model.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CAction; }
template void CCountedPointer<NUISystem::CAction>::Reset(NUISystem::CAction*);
