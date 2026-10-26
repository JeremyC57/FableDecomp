// CTCHeroRangedAttacks::StatGetClassTypeName() -- static: the class-type name "CTCHeroRangedAttacks" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroRangedAttacks@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4A30: push esi; push -1; push "CTCHeroRangedAttacks"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123c234 = "CTCHeroRangedAttacks" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroRangedAttacks@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4a30::build).
#define CTCHeroRangedAttacks CTCHeroRangedAttacksLayout
#include "engine/CTCHeroRangedAttacks.h"
#undef CTCHeroRangedAttacks

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroRangedAttacks : public CTCHeroRangedAttacksLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroRangedAttacks::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123C234, -1);   // "CTCHeroRangedAttacks"
}
