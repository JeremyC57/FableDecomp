// CTCBerserk::StatGetClassTypeName() -- static: the class-type name "CTCBerserk" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBerserk@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4A86: push esi; push -1; push "CTCBerserk"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c274 = "CTCBerserk" = the class name; PDB name set ?StatGetClassTypeName@CTCBerserk@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4a86::build).
#define CTCBerserk CTCBerserkLayout
#include "engine/CTCBerserk.h"
#undef CTCBerserk

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBerserk : public CTCBerserkLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBerserk::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C274, -1);   // "CTCBerserk"
}
