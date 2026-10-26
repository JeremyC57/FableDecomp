// CTCInGameMenu::StatGetClassTypeName() -- static: the class-type name "CTCInGameMenu" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCInGameMenu@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D55BD: push esi; push -1; push "CTCInGameMenu"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x1236874 = "CTCInGameMenu" = the class name; PDB name set ?StatGetClassTypeName@CTCInGameMenu@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d55bd::build).
#define CTCInGameMenu CTCInGameMenuLayout
#include "engine/CTCInGameMenu.h"
#undef CTCInGameMenu

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCInGameMenu : public CTCInGameMenuLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCInGameMenu::StatGetClassTypeName()
{
    return CCharString((const char*)0x01236874, -1);   // "CTCInGameMenu"
}
