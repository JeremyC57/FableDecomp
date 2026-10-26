// CTCHeroAttachableAppearanceModifiers::StatGetClassTypeName() -- static: the class-type name "CTCHeroAttachableAppearanceModifiers" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroAttachableAppearanceModifiers@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4535: push esi; push -1; push "CTCHeroAttachableAppearanceModifiers"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123bcd0 = "CTCHeroAttachableAppearanceModifiers" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroAttachableAppearanceModifiers@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d4535::build).
#define CTCHeroAttachableAppearanceModifiers CTCHeroAttachableAppearanceModifiersLayout
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#undef CTCHeroAttachableAppearanceModifiers

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroAttachableAppearanceModifiers : public CTCHeroAttachableAppearanceModifiersLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroAttachableAppearanceModifiers::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BCD0, -1);   // "CTCHeroAttachableAppearanceModifiers"
}
