// CTCTurncoatSpell::StatGetClassTypeName() -- static: the class-type name "CTCTurncoatSpell" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCTurncoatSpell@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D56B0: push esi; push -1; push "CTCTurncoatSpell"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123d1b8 = "CTCTurncoatSpell" = the class name; PDB name set ?StatGetClassTypeName@CTCTurncoatSpell@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d56b0::build).
#define CTCTurncoatSpell CTCTurncoatSpellLayout
#include "engine/CTCTurncoatSpell.h"
#undef CTCTurncoatSpell

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCTurncoatSpell : public CTCTurncoatSpellLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCTurncoatSpell::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123D1B8, -1);   // "CTCTurncoatSpell"
}
