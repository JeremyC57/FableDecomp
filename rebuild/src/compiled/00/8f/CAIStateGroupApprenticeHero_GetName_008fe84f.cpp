// CAIStateGroup_ApprenticeHero::GetName() -- static: the AI state group name "SG_APPRENTICE_HERO" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ApprenticeHero@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE84F: push esi; push -1; push "SG_APPRENTICE_HERO"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12216f4 registers this GetName with Alloc 0x901d93 (pushed before it); Alloc 0x901d93 -> ctor 0x8fe862 stores (last) the vtable of CAIStateGroup_ApprenticeHero at this+0; literal "SG_APPRENTICE_HERO"; PDB name set ?GetName@CAIStateGroup_ApprenticeHero@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ApprenticeHero@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe84f::build).
#define CAIStateGroup_ApprenticeHero CAIStateGroup_ApprenticeHeroLayout
#include "engine/CAIStateGroup_ApprenticeHero.h"
#undef CAIStateGroup_ApprenticeHero

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ApprenticeHero : public CAIStateGroup_ApprenticeHeroLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ApprenticeHero::GetName()
{
    return CCharString((const char*)0x0128AEAC, -1);   // "SG_APPRENTICE_HERO"
}
