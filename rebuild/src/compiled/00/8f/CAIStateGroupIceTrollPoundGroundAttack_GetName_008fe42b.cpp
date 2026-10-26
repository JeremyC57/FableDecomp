// CAIStateGroup_IceTrollPoundGroundAttack::GetName() -- static: the AI state group name "SG_ICE_TROLL_POUND_GROUND_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_IceTrollPoundGroundAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE42B: push esi; push -1; push "SG_ICE_TROLL_POUND_GROUND_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220a9c registers this GetName with Alloc 0x902b48 (pushed before it); Alloc 0x902b48 -> ctor 0x90135e stores (last) the vtable of CAIStateGroup_IceTrollPoundGroundAttack at this+0; literal "SG_ICE_TROLL_POUND_GROUND_ATTACK"; PDB name set ?GetName@CAIStateGroup_IceTrollPoundGroundAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_IceTrollPoundGroundAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe42b::build).
#define CAIStateGroup_IceTrollPoundGroundAttack CAIStateGroup_IceTrollPoundGroundAttackLayout
#include "engine/CAIStateGroup_IceTrollPoundGroundAttack.h"
#undef CAIStateGroup_IceTrollPoundGroundAttack

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_IceTrollPoundGroundAttack : public CAIStateGroup_IceTrollPoundGroundAttackLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_IceTrollPoundGroundAttack::GetName()
{
    return CCharString((const char*)0x0128A790, -1);   // "SG_ICE_TROLL_POUND_GROUND_ATTACK"
}
