// CAIStateGroup_MeleeCombatNearby2::GetName() -- static: the AI state group name "SG_MELEE_COMBAT_NEARBY2" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MeleeCombatNearby2@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD8EA: push esi; push -1; push "SG_MELEE_COMBAT_NEARBY2"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220074 registers this GetName with Alloc 0x90236e (pushed before it); Alloc 0x90236e stores the vtable of CAIStateGroup_MeleeCombatNearby2 into the new object; literal "SG_MELEE_COMBAT_NEARBY2"; PDB name set ?GetName@CAIStateGroup_MeleeCombatNearby2@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MeleeCombatNearby2@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd8ea::build).
#define CAIStateGroup_MeleeCombatNearby2 CAIStateGroup_MeleeCombatNearby2Layout
#include "engine/CAIStateGroup_MeleeCombatNearby2.h"
#undef CAIStateGroup_MeleeCombatNearby2

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MeleeCombatNearby2 : public CAIStateGroup_MeleeCombatNearby2Layout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MeleeCombatNearby2::GetName()
{
    return CCharString((const char*)0x0127810C, -1);   // "SG_MELEE_COMBAT_NEARBY2"
}
