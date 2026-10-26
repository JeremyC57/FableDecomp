// CAIStateGroup_MinionFollowThing::GetName() -- static: the AI state group name "SG_MINION_FOLLOW_THING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionFollowThing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE49D: push esi; push -1; push "SG_MINION_FOLLOW_THING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122121c registers this GetName with Alloc 0x902bb8 (pushed before it); Alloc 0x902bb8 -> ctor 0x90148c stores (last) the vtable of CAIStateGroup_MinionFollowThing at this+0; literal "SG_MINION_FOLLOW_THING"; PDB name set ?GetName@CAIStateGroup_MinionFollowThing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionFollowThing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe49d::build).
#define CAIStateGroup_MinionFollowThing CAIStateGroup_MinionFollowThingLayout
#include "engine/CAIStateGroup_MinionFollowThing.h"
#undef CAIStateGroup_MinionFollowThing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionFollowThing : public CAIStateGroup_MinionFollowThingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionFollowThing::GetName()
{
    return CCharString((const char*)0x0128A814, -1);   // "SG_MINION_FOLLOW_THING"
}
