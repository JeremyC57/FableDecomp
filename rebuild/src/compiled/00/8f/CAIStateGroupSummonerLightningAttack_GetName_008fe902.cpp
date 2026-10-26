// CAIStateGroup_SummonerLightningAttack::GetName() -- static: the AI state group name "SG_SUMMONER_LIGHTNING_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SummonerLightningAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE902: push esi; push -1; push "SG_SUMMONER_LIGHTNING_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221794 registers this GetName with Alloc 0x902ebd (pushed before it); Alloc 0x902ebd -> ctor 0x902d03 stores (last) the vtable of CAIStateGroup_SummonerLightningAttack at this+0; literal "SG_SUMMONER_LIGHTNING_ATTACK"; PDB name set ?GetName@CAIStateGroup_SummonerLightningAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SummonerLightningAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe902::build).
#define CAIStateGroup_SummonerLightningAttack CAIStateGroup_SummonerLightningAttackLayout
#include "engine/CAIStateGroup_SummonerLightningAttack.h"
#undef CAIStateGroup_SummonerLightningAttack

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SummonerLightningAttack : public CAIStateGroup_SummonerLightningAttackLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SummonerLightningAttack::GetName()
{
    return CCharString((const char*)0x0128AFF8, -1);   // "SG_SUMMONER_LIGHTNING_ATTACK"
}
