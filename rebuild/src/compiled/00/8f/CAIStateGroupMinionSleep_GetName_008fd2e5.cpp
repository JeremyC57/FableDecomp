// CAIStateGroup_MinionSleep::GetName() -- static: the AI state group name "SG_MINION_SLEEP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionSleep@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD2E5: push esi; push -1; push "SG_MINION_SLEEP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f8a4 registers this GetName with Alloc 0x901f0d (pushed before it); Alloc 0x901f0d stores the vtable of CAIStateGroup_MinionSleep into the new object; literal "SG_MINION_SLEEP"; PDB name set ?GetName@CAIStateGroup_MinionSleep@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionSleep@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd2e5::build).
#define CAIStateGroup_MinionSleep CAIStateGroup_MinionSleepLayout
#include "engine/CAIStateGroup_MinionSleep.h"
#undef CAIStateGroup_MinionSleep

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionSleep : public CAIStateGroup_MinionSleepLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionSleep::GetName()
{
    return CCharString((const char*)0x0127814C, -1);   // "SG_MINION_SLEEP"
}
