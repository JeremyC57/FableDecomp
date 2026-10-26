// CTCBoastUI::StatGetClassTypeName() -- static: the class-type name "CTCBoastUI" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBoastUI@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5FC8: push esi; push -1; push "CTCBoastUI"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123dd08 = "CTCBoastUI" = the class name; PDB name set ?StatGetClassTypeName@CTCBoastUI@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d5fc8::build).
#define CTCBoastUI CTCBoastUILayout
#include "engine/CTCBoastUI.h"
#undef CTCBoastUI

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBoastUI : public CTCBoastUILayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBoastUI::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123DD08, -1);   // "CTCBoastUI"
}
