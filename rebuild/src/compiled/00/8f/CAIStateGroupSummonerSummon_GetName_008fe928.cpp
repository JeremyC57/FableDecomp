// CAIStateGroup_SummonerSummon::GetName() -- static: the AI state group name "SG_SUMMONER_SUMMON" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SummonerSummon@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE928: push esi; push -1; push "SG_SUMMONER_SUMMON"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12217bc registers this GetName with Alloc 0x902d5b (pushed before it); Alloc 0x902d5b -> ctor 0x901e85 stores (last) the vtable of CAIStateGroup_SummonerSummon at this+0; literal "SG_SUMMONER_SUMMON"; PDB name set ?GetName@CAIStateGroup_SummonerSummon@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SummonerSummon@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe928::build).
#define CAIStateGroup_SummonerSummon CAIStateGroup_SummonerSummonLayout
#include "engine/CAIStateGroup_SummonerSummon.h"
#undef CAIStateGroup_SummonerSummon

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SummonerSummon : public CAIStateGroup_SummonerSummonLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SummonerSummon::GetName()
{
    return CCharString((const char*)0x0128B02C, -1);   // "SG_SUMMONER_SUMMON"
}
