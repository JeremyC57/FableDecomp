// std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >::`scalar deleting destructor' -- destroy the pair (out-of-line ~pair 0x00429E16), free it when flags & 1, return this
// Leaf: ??_G?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A8E0, 28 bytes, thiscall, `ret 4`: `push esi ; mov esi,ecx ; call 0x00429E16 ; test byte ptr [esp+8],1 ;
//   je ; push esi ; call 0x00BFE9BC (operator delete) ; pop ecx ; mov eax,esi ; pop esi`.
// Identity (NF230, 2026-11-22, q12620): Ego_r.pdb and FableWin.pdb both name ??_G?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAEPAXI@Z
//   (S_GPROC32 in ui_manager.obj, NUISystem::CManager's TU); its call goes to 0x00429E16, the landed ~pair of the same pair. Was landed as
//   the descriptive extern "C" @CAssoc_scalar_deleting_dtor@12 (catalog _global::ScalarDeletingDtor_0042a8e0, a propagated CAssoc label)
//   calling the stand-in @CAssoc_dtor@4. The value type of NUISystem::CManager's BastardChildren map<CCharString, CCountedPointer<CComponent> >.
// Model: engine/NUISystem_CManager.h (the map's value type), stlport/fable_containers.h _Destroy. Instantiating the pair's destructor
//   (the explicit instantiation of std::_Destroy, the call the tree's _M_erase 0x0042B8FC makes) makes VC7.1 emit the pair's ??_G COMDAT
//   with it (as in 0x0042B8FC's object, whose ??_G copy NF229 measured equal to these bytes). Defines and pragma as 0x0042B8FC (same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Destroy(std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >*);
