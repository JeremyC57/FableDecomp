// CTCHeroCentre::StatGetClassTypeName() -- static: the class-type name "CTCHeroCentre" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroCentre@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4617: push esi; push -1; push "CTCHeroCentre"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123be3c = "CTCHeroCentre" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroCentre@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4617::build).
#define CTCHeroCentre CTCHeroCentreLayout
#include "engine/CTCHeroCentre.h"
#undef CTCHeroCentre

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroCentre : public CTCHeroCentreLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroCentre::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BE3C, -1);   // "CTCHeroCentre"
}
