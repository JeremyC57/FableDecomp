// CAIStateGroup_CreatureRangedCombatUseWillShield::GetName() -- static: the AI state group name "SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureRangedCombatUseWillShield@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE25F: push esi; push -1; push "SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122018c registers this GetName with Alloc 0x900e6c (pushed before it); Alloc 0x900e6c stores the vtable of CAIStateGroup_CreatureRangedCombatUseWillShield into the new object; literal "SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD"; PDB name set ?GetName@CAIStateGroup_CreatureRangedCombatUseWillShield@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureRangedCombatUseWillShield@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe25f::build).
#define CAIStateGroup_CreatureRangedCombatUseWillShield CAIStateGroup_CreatureRangedCombatUseWillShieldLayout
#include "engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h"
#undef CAIStateGroup_CreatureRangedCombatUseWillShield

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureRangedCombatUseWillShield : public CAIStateGroup_CreatureRangedCombatUseWillShieldLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureRangedCombatUseWillShield::GetName()
{
    return CCharString((const char*)0x0128A454, -1);   // "SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD"
}
