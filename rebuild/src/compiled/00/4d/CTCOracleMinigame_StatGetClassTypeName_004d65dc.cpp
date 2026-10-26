// CTCOracleMinigame::StatGetClassTypeName() -- static: the class-type name "CTCOracleMinigame" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCOracleMinigame@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D65DC: push esi; push -1; push "CTCOracleMinigame"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123e4ec = "CTCOracleMinigame" = the class name; PDB name set ?StatGetClassTypeName@CTCOracleMinigame@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d65dc::build).
#define CTCOracleMinigame CTCOracleMinigameLayout
#include "engine/CTCOracleMinigame.h"
#undef CTCOracleMinigame

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCOracleMinigame : public CTCOracleMinigameLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCOracleMinigame::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123E4EC, -1);   // "CTCOracleMinigame"
}
