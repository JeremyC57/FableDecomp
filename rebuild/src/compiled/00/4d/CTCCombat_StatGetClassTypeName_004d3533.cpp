// CTCCombat::StatGetClassTypeName() -- static: the class-type name "CTCCombat" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCombat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3533: push esi; push -1; push "CTCCombat"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123aa3c = "CTCCombat" = the class name; PDB name set ?StatGetClassTypeName@CTCCombat@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d3533::build).
#define CTCCombat CTCCombatLayout
#include "engine/CTCCombat.h"
#undef CTCCombat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCombat : public CTCCombatLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCombat::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AA3C, -1);   // "CTCCombat"
}
