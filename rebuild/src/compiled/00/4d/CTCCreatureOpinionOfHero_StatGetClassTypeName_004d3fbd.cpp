// CTCCreatureOpinionOfHero::StatGetClassTypeName() -- static: the class-type name "CTCCreatureOpinionOfHero" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCreatureOpinionOfHero@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3FBD: push esi; push -1; push "CTCCreatureOpinionOfHero"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123b600 = "CTCCreatureOpinionOfHero" = the class name; PDB name set ?StatGetClassTypeName@CTCCreatureOpinionOfHero@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3fbd::build).
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroLayout
#include "engine/CTCCreatureOpinionOfHero.h"
#undef CTCCreatureOpinionOfHero

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCreatureOpinionOfHero : public CTCCreatureOpinionOfHeroLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCreatureOpinionOfHero::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123B600, -1);   // "CTCCreatureOpinionOfHero"
}
