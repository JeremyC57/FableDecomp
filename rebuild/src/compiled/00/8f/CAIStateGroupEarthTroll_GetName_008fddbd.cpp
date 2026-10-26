// CAIStateGroup_EarthTroll::GetName() -- static: the AI state group name "SG_EARTH_TROLL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_EarthTroll@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDDBD: push esi; push -1; push "SG_EARTH_TROLL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220a24 registers this GetName with Alloc 0x902620 (pushed before it); Alloc 0x902620 stores the vtable of CAIStateGroup_EarthTroll into the new object; literal "SG_EARTH_TROLL"; PDB name set ?GetName@CAIStateGroup_EarthTroll@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_EarthTroll@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fddbd::build).
#define CAIStateGroup_EarthTroll CAIStateGroup_EarthTrollLayout
#include "engine/CAIStateGroup_EarthTroll.h"
#undef CAIStateGroup_EarthTroll

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_EarthTroll : public CAIStateGroup_EarthTrollLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_EarthTroll::GetName()
{
    return CCharString((const char*)0x01289CF8, -1);   // "SG_EARTH_TROLL"
}
