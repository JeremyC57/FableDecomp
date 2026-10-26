// CAIStateGroup_MeleeCombatNearby::GetName() -- static: the AI state group name "SG_MELEE_COMBAT_NEARBY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MeleeCombatNearby@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD8C4: push esi; push -1; push "SG_MELEE_COMBAT_NEARBY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122004c registers this GetName with Alloc 0x902db9 (pushed before it); Alloc 0x902db9 stores the vtable of CAIStateGroup_MeleeCombatNearby into the new object; literal "SG_MELEE_COMBAT_NEARBY"; PDB name set ?GetName@CAIStateGroup_MeleeCombatNearby@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MeleeCombatNearby@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd8c4::build).
#define CAIStateGroup_MeleeCombatNearby CAIStateGroup_MeleeCombatNearbyLayout
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#undef CAIStateGroup_MeleeCombatNearby

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MeleeCombatNearby : public CAIStateGroup_MeleeCombatNearbyLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MeleeCombatNearby::GetName()
{
    return CCharString((const char*)0x01278124, -1);   // "SG_MELEE_COMBAT_NEARBY"
}
