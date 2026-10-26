// CTCCreatureGenerator::StatGetClassTypeName() -- static: the class-type name "CTCCreatureGenerator" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCCreatureGenerator@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D37B3: push esi; push -1; push "CTCCreatureGenerator"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123ac98 = "CTCCreatureGenerator" = the class name; PDB name set ?StatGetClassTypeName@CTCCreatureGenerator@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d37b3::build).
#define CTCCreatureGenerator CTCCreatureGeneratorLayout
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCCreatureGenerator : public CTCCreatureGeneratorLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCCreatureGenerator::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AC98, -1);   // "CTCCreatureGenerator"
}
