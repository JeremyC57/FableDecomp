// CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::GetName() -- static: the AI state group name "SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE2D3: push esi; push -1; push "SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220e0c registers this GetName with Alloc 0x902a5e (pushed before it); Alloc 0x902a5e -> ctor 0x900f5e stores (last) the vtable of CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets at this+0; literal "SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS"; PDB name set ?GetName@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe2d3::build).
#define CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets CAIStateGroup_CreatureRangedCombatFireAtSpecificTargetsLayout
#include "engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h"
#undef CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets : public CAIStateGroup_CreatureRangedCombatFireAtSpecificTargetsLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::GetName()
{
    return CCharString((const char*)0x0128A56C, -1);   // "SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS"
}
