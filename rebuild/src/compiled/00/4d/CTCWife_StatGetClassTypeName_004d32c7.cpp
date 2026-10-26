// CTCWife::StatGetClassTypeName() -- static: the class-type name "CTCWife" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCWife@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D32C7: push esi; push -1; push "CTCWife"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a700 = "CTCWife" = the class name; PDB name set ?StatGetClassTypeName@CTCWife@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d32c7::build).
#define CTCWife CTCWifeLayout
#include "engine/CTCWife.h"
#undef CTCWife

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCWife : public CTCWifeLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCWife::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A700, -1);   // "CTCWife"
}
