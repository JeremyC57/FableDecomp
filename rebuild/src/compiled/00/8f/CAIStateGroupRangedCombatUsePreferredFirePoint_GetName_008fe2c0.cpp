// CAIStateGroup_RangedCombatUsePreferredFirePoint::GetName() -- static: the AI state group name "SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_RangedCombatUsePreferredFirePoint@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE2C0: push esi; push -1; push "SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220164 registers this GetName with Alloc 0x902a48 (pushed before it); Alloc 0x902a48 -> ctor 0x900eec stores (last) the vtable of CAIStateGroup_RangedCombatUsePreferredFirePoint at this+0; literal "SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT"; PDB name set ?GetName@CAIStateGroup_RangedCombatUsePreferredFirePoint@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_RangedCombatUsePreferredFirePoint@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe2c0::build).
#define CAIStateGroup_RangedCombatUsePreferredFirePoint CAIStateGroup_RangedCombatUsePreferredFirePointLayout
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"
#undef CAIStateGroup_RangedCombatUsePreferredFirePoint

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_RangedCombatUsePreferredFirePoint : public CAIStateGroup_RangedCombatUsePreferredFirePointLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_RangedCombatUsePreferredFirePoint::GetName()
{
    return CCharString((const char*)0x0128A538, -1);   // "SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT"
}
