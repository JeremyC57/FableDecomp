// CTCBoastingPosition::StatGetClassTypeName() -- static: the class-type name "CTCBoastingPosition" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCBoastingPosition@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4716: push esi; push -1; push "CTCBoastingPosition"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bf70 = "CTCBoastingPosition" = the class name; PDB name set ?StatGetClassTypeName@CTCBoastingPosition@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4716::build).
#define CTCBoastingPosition CTCBoastingPositionLayout
#include "engine/CTCBoastingPosition.h"
#undef CTCBoastingPosition

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBoastingPosition : public CTCBoastingPositionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCBoastingPosition::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BF70, -1);   // "CTCBoastingPosition"
}
