// CAIStateGroup_MinionWander::GetName() -- static: the AI state group name "SG_MINION_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD242: push esi; push -1; push "SG_MINION_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f87c registers this GetName with Alloc 0x901ed3 (pushed before it); Alloc 0x901ed3 stores the vtable of CAIStateGroup_MinionWander into the new object; literal "SG_MINION_WANDER"; PDB name set ?GetName@CAIStateGroup_MinionWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd242::build).
#define CAIStateGroup_MinionWander CAIStateGroup_MinionWanderLayout
#include "engine/CAIStateGroup_MinionWander.h"
#undef CAIStateGroup_MinionWander

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionWander : public CAIStateGroup_MinionWanderLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionWander::GetName()
{
    return CCharString((const char*)0x01288C48, -1);   // "SG_MINION_WANDER"
}
