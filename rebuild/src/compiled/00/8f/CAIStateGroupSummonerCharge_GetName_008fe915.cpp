// CAIStateGroup_SummonerCharge::GetName() -- static: the AI state group name "SG_SUMMONER_CHARGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SummonerCharge@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE915: push esi; push -1; push "SG_SUMMONER_CHARGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12217e4 registers this GetName with Alloc 0x902f17 (pushed before it); Alloc 0x902f17 -> ctor 0x902d2f stores (last) the vtable of CAIStateGroup_SummonerCharge at this+0; literal "SG_SUMMONER_CHARGE"; PDB name set ?GetName@CAIStateGroup_SummonerCharge@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SummonerCharge@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe915::build).
#define CAIStateGroup_SummonerCharge CAIStateGroup_SummonerChargeLayout
#include "engine/CAIStateGroup_SummonerCharge.h"
#undef CAIStateGroup_SummonerCharge

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SummonerCharge : public CAIStateGroup_SummonerChargeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SummonerCharge::GetName()
{
    return CCharString((const char*)0x0128B018, -1);   // "SG_SUMMONER_CHARGE"
}
