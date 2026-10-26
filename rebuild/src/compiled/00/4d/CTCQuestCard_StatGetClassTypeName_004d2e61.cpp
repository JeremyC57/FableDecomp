// CTCQuestCard::StatGetClassTypeName() -- static: the class-type name "CTCQuestCard" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCQuestCard@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2E61: push esi; push -1; push "CTCQuestCard"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123a3f0 = "CTCQuestCard" = the class name; PDB name set ?StatGetClassTypeName@CTCQuestCard@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d2e61::build).
#define CTCQuestCard CTCQuestCardLayout
#include "engine/CTCQuestCard.h"
#undef CTCQuestCard

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCQuestCard : public CTCQuestCardLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCQuestCard::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123A3F0, -1);   // "CTCQuestCard"
}
