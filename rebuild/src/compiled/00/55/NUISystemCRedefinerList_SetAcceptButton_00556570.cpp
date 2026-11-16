// Leaf: ?SetAcceptButton@CRedefinerList@NUISystem@@QAEXPAVCComponent@2@@Z
// Name-fix NF38: Ego_r.pdb ?SetAcceptButton@CRedefinerList@NUISystem@@QAEXPAVCComponent@2@@Z (public), declared for 0x00556570 by
// engine/NUISystem_CRedefinerList.h FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON (PAcceptButton +0x18C); the landed caller 0x0058AF10 binds
// it. Was the free stand-in AddExclusiveObserver(CObservable*, edx, observer).
// NUISystem::CRedefinerList::SetAcceptButton @ 0x00556570
// mov eax,[esp+4]; mov [ecx+0x18c],eax; ret 4
// thiscall: store the accept button pointer into PAcceptButton at +0x18C.
#define FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON
#include "engine/NUISystem_CRedefinerList.h"

void NUISystem::CRedefinerList::SetAcceptButton(CComponent* button)
{
    PAcceptButton = button;
}
