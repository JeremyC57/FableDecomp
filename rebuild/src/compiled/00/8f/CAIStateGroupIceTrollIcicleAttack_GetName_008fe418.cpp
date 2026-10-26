// CAIStateGroup_IceTrollIcicleAttack::GetName() -- static: the AI state group name "SG_ICE_TROLL_ICICLE_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_IceTrollIcicleAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE418: push esi; push -1; push "SG_ICE_TROLL_ICICLE_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220a74 registers this GetName with Alloc 0x902b32 (pushed before it); Alloc 0x902b32 -> ctor 0x901310 stores (last) the vtable of CAIStateGroup_IceTrollIcicleAttack at this+0; literal "SG_ICE_TROLL_ICICLE_ATTACK"; PDB name set ?GetName@CAIStateGroup_IceTrollIcicleAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_IceTrollIcicleAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe418::build).
#define CAIStateGroup_IceTrollIcicleAttack CAIStateGroup_IceTrollIcicleAttackLayout
#include "engine/CAIStateGroup_IceTrollIcicleAttack.h"
#undef CAIStateGroup_IceTrollIcicleAttack

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_IceTrollIcicleAttack : public CAIStateGroup_IceTrollIcicleAttackLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_IceTrollIcicleAttack::GetName()
{
    return CCharString((const char*)0x0128A774, -1);   // "SG_ICE_TROLL_ICICLE_ATTACK"
}
