// CAIStateGroup_HoverWander::GetName() -- static: the AI state group name "SG_HOVER_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE13A: push esi; push -1; push "SG_HOVER_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12208bc registers this GetName with Alloc 0x90295a (pushed before it); Alloc 0x90295a stores the vtable of CAIStateGroup_HoverWander into the new object; literal "SG_HOVER_WANDER"; PDB name set ?GetName@CAIStateGroup_HoverWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe13a::build).
#define CAIStateGroup_HoverWander CAIStateGroup_HoverWanderLayout
#include "engine/CAIStateGroup_HoverWander.h"
#undef CAIStateGroup_HoverWander

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverWander : public CAIStateGroup_HoverWanderLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverWander::GetName()
{
    return CCharString((const char*)0x0128A258, -1);   // "SG_HOVER_WANDER"
}
