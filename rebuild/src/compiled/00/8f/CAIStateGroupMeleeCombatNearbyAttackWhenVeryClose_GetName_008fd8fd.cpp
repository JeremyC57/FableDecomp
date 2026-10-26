// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::GetName() -- static: the AI state group name "SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD8FD: push esi; push -1; push "SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122009c registers this GetName with Alloc 0x902392 (pushed before it); Alloc 0x902392 stores the vtable of CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose into the new object; literal "SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"; PDB name set ?GetName@CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd8fd::build).
#define CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose CAIStateGroup_MeleeCombatNearbyAttackWhenVeryCloseLayout
#include "engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h"
#undef CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose : public CAIStateGroup_MeleeCombatNearbyAttackWhenVeryCloseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::GetName()
{
    return CCharString((const char*)0x012780DC, -1);   // "SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"
}
