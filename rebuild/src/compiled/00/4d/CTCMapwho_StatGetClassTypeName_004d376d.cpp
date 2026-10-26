// CTCMapwho::StatGetClassTypeName() -- static: the class-type name "CTCMapwho" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCMapwho@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D376D: push esi; push -1; push "CTCMapwho"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x1239be4 = "CTCMapwho" = the class name; PDB name set ?StatGetClassTypeName@CTCMapwho@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d376d::build).
#define CTCMapwho CTCMapwhoLayout
#include "engine/CTCMapwho.h"
#undef CTCMapwho

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCMapwho : public CTCMapwhoLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCMapwho::StatGetClassTypeName()
{
    return CCharString((const char*)0x01239BE4, -1);   // "CTCMapwho"
}
