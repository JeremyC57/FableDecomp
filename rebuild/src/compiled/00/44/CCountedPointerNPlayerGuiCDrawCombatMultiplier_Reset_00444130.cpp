// CCountedPointer::Reset(T*) -- release the old pointee, take the new one and give it a fresh count block (T = NPlayerGui::CDrawCombatMultiplier)
// Leaf: ?Reset@?$CCountedPointer@VCDrawCombatMultiplier@NPlayerGui@@@@QAEXPAVCDrawCombatMultiplier@NPlayerGui@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00444130, 103 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00444130 (stand-in name).
// T = NPlayerGui::CDrawCombatMultiplier, proven by: CPlayerGui::CPlayerGui 0x0043B570 binds Reset<NPlayerGui::CDrawCombatMultiplier> (PDrawCombatMultiplier: Ego_r CPlayerGui CCountedPointer<NPlayerGui::CDrawCombatMultiplier>); the stored deleter 0x00443540 is the non-virtual DeleteData (inline member destruction), next to DeleteData<CDrawHealth> 0x00443500; FableWin.pdb Reset name.
// Member: the in-class Reset of engine/CCountedPointer.h (ReleaseData expanded; its Data store dies under Data = data), own COMDAT.
// The count block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NPlayerGui { class CDrawCombatMultiplier; }
template void CCountedPointer<NPlayerGui::CDrawCombatMultiplier>::Reset(NPlayerGui::CDrawCombatMultiplier*);
