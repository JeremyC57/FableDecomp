// CAIStateGroup_HoverWanderAroundThing::GetName() -- static: the AI state group name "SG_HOVER_WANDER_AROUND_THING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverWanderAroundThing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE127: push esi; push -1; push "SG_HOVER_WANDER_AROUND_THING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12208e4 registers this GetName with Alloc 0x902e5f (pushed before it); Alloc 0x902e5f -> ctor 0x902907 stores (last) the vtable of CAIStateGroup_HoverWanderAroundThing at this+0; literal "SG_HOVER_WANDER_AROUND_THING"; PDB name set ?GetName@CAIStateGroup_HoverWanderAroundThing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverWanderAroundThing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe127::build).
#define CAIStateGroup_HoverWanderAroundThing CAIStateGroup_HoverWanderAroundThingLayout
#include "engine/CAIStateGroup_HoverWanderAroundThing.h"
#undef CAIStateGroup_HoverWanderAroundThing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverWanderAroundThing : public CAIStateGroup_HoverWanderAroundThingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverWanderAroundThing::GetName()
{
    return CCharString((const char*)0x0128A238, -1);   // "SG_HOVER_WANDER_AROUND_THING"
}
