// CTCHeroExperience::StatGetClassTypeName() -- static: the class-type name "CTCHeroExperience" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroExperience@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2F96: push esi; push -1; push "CTCHeroExperience"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a460 = "CTCHeroExperience" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroExperience@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d2f96::build).
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroExperience : public CTCHeroExperienceLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroExperience::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A460, -1);   // "CTCHeroExperience"
}
