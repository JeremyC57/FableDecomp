// CTCHeroStats::StatGetClassTypeName() -- static: the class-type name "CTCHeroStats" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroStats@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2FC9: push esi; push -1; push "CTCHeroStats"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a474 = "CTCHeroStats" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroStats@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d2fc9::build).
#define CTCHeroStats CTCHeroStatsLayout
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroStats : public CTCHeroStatsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroStats::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A474, -1);   // "CTCHeroStats"
}
