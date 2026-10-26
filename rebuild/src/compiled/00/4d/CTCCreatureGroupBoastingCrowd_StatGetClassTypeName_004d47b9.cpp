// CTCCreatureGroupBoastingCrowd::StatGetClassTypeName() -- static: the class-type name "CTCCreatureGroupBoastingCrowd" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCreatureGroupBoastingCrowd@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D47B9: push esi; push -1; push "CTCCreatureGroupBoastingCrowd"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bfc0 = "CTCCreatureGroupBoastingCrowd" = the class name; PDB name set ?StatGetClassTypeName@CTCCreatureGroupBoastingCrowd@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d47b9::build).
#define CTCCreatureGroupBoastingCrowd CTCCreatureGroupBoastingCrowdLayout
#include "engine/CTCCreatureGroupBoastingCrowd.h"
#undef CTCCreatureGroupBoastingCrowd

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCreatureGroupBoastingCrowd : public CTCCreatureGroupBoastingCrowdLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCreatureGroupBoastingCrowd::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BFC0, -1);   // "CTCCreatureGroupBoastingCrowd"
}
