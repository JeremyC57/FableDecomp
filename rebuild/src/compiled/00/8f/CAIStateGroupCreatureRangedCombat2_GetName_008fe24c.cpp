// CAIStateGroup_CreatureRangedCombat2::GetName() -- static: the AI state group name "SG_CREATURE_RANGED_COMBAT_TWO" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureRangedCombat2@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE24C: push esi; push -1; push "SG_CREATURE_RANGED_COMBAT_TWO"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220d94 registers this GetName with Alloc 0x902a32 (pushed before it); Alloc 0x902a32 -> ctor 0x900e1e stores (last) the vtable of CAIStateGroup_CreatureRangedCombat2 at this+0; literal "SG_CREATURE_RANGED_COMBAT_TWO"; PDB name set ?GetName@CAIStateGroup_CreatureRangedCombat2@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureRangedCombat2@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe24c::build).
#define CAIStateGroup_CreatureRangedCombat2 CAIStateGroup_CreatureRangedCombat2Layout
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"
#undef CAIStateGroup_CreatureRangedCombat2

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureRangedCombat2 : public CAIStateGroup_CreatureRangedCombat2Layout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureRangedCombat2::GetName()
{
    return CCharString((const char*)0x0128A434, -1);   // "SG_CREATURE_RANGED_COMBAT_TWO"
}
