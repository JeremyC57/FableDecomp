// CAIStateGroup_RockTroll::GetName() -- static: the AI state group name "SG_ROCK_TROLL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_RockTroll@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDD97: push esi; push -1; push "SG_ROCK_TROLL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220a4c registers this GetName with Alloc 0x902ddd (pushed before it); Alloc 0x902ddd stores the vtable of CAIStateGroup_RockTroll into the new object; literal "SG_ROCK_TROLL"; PDB name set ?GetName@CAIStateGroup_RockTroll@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_RockTroll@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdd97::build).
#define CAIStateGroup_RockTroll CAIStateGroup_RockTrollLayout
#include "engine/CAIStateGroup_RockTroll.h"
#undef CAIStateGroup_RockTroll

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_RockTroll : public CAIStateGroup_RockTrollLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_RockTroll::GetName()
{
    return CCharString((const char*)0x01289CE8, -1);   // "SG_ROCK_TROLL"
}
