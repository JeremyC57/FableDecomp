// CInputProcessUseRangedWeaponZLock::Update(CProcessedInput& processed_input) -- calls CPlayer::GetPControlledCreature @ 0x00487DC0 on MyPlayer (+0x18) and drops the result; the argument is unused
// Leaf: ?Update@CInputProcessUseRangedWeaponZLock@@UAEXAAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068EB10, oracle length 11 bytes. CInputProcessUseRangedWeaponZLock vftable slot 6 (RTTI; NF477 audit:
// one `.rdata` dword, no call). Identity: Ego_r.pdb + FableWin.pdb ?Update@CInputProcessUseRangedWeaponZLock@@UAEXAAVCProcessedInput@@@Z (public virtual).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was ?Update@CInputProcessUseRangedWeaponZLock@@QAEXJ@Z on a local class model (generated tiny-body
//   shape, `long` argument); respelt onto engine/CInputProcessUseRangedWeaponZLock.h FABLE_RETAIL_INPUTPROCESS_VTABLE (MyPlayer is
//   CInputProcessGameBase::MyPlayer). No body change.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 0x18] ; call 0x487dc0 ; ret 4
class CThingPlayerCreature;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessUseRangedWeaponZLock.h"

void CInputProcessUseRangedWeaponZLock::Update(CProcessedInput&)
{
    MyPlayer->GetPControlledCreature();
}
