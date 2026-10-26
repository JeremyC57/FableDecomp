// CTCDoor::StatGetClassTypeName() -- static: the class-type name "CTCDoor" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCDoor@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D32F7: push esi; push -1; push "CTCDoor"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a708 = "CTCDoor" = the class name; PDB name set ?StatGetClassTypeName@CTCDoor@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d32f7::build).
#define CTCDoor CTCDoorLayout
#include "engine/CTCDoor.h"
#undef CTCDoor

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCDoor : public CTCDoorLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCDoor::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A708, -1);   // "CTCDoor"
}
