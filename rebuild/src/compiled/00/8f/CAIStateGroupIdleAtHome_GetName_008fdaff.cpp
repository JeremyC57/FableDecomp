// CAIStateGroup_IdleAtHome::GetName() -- static: the AI state group name "SG_IDLE_AT_HOME" as a CCharString
// Leaf: ?GetName@CAIStateGroup_IdleAtHome@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDAFF: push esi; push -1; push "SG_IDLE_AT_HOME"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220344 registers this GetName with Alloc 0x8ffe98 (pushed before it); Alloc 0x8ffe98 stores the vtable of CAIStateGroup_IdleAtHome into the new object; literal "SG_IDLE_AT_HOME"; PDB name set ?GetName@CAIStateGroup_IdleAtHome@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_IdleAtHome@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdaff::build).
#define CAIStateGroup_IdleAtHome CAIStateGroup_IdleAtHomeLayout
#include "engine/CAIStateGroup_IdleAtHome.h"
#undef CAIStateGroup_IdleAtHome

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_IdleAtHome : public CAIStateGroup_IdleAtHomeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_IdleAtHome::GetName()
{
    return CCharString((const char*)0x012899FC, -1);   // "SG_IDLE_AT_HOME"
}
