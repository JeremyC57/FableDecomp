// CAIStateGroup_MinionGuardReactToCriminal::GetName() -- static: the AI state group name "SG_MINION_GUARD_REACT_TO_CRIMINAL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardReactToCriminal@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD356: push esi; push -1; push "SG_MINION_GUARD_REACT_TO_CRIMINAL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fa34 registers this GetName with Alloc 0x901f81 (pushed before it); Alloc 0x901f81 -> ctor 0x8fed07 stores (last) the vtable of CAIStateGroup_MinionGuardReactToCriminal at this+0; literal "SG_MINION_GUARD_REACT_TO_CRIMINAL"; PDB name set ?GetName@CAIStateGroup_MinionGuardReactToCriminal@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardReactToCriminal@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd356::build).
#define CAIStateGroup_MinionGuardReactToCriminal CAIStateGroup_MinionGuardReactToCriminalLayout
#include "engine/CAIStateGroup_MinionGuardReactToCriminal.h"
#undef CAIStateGroup_MinionGuardReactToCriminal

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardReactToCriminal : public CAIStateGroup_MinionGuardReactToCriminalLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardReactToCriminal::GetName()
{
    return CCharString((const char*)0x01288D64, -1);   // "SG_MINION_GUARD_REACT_TO_CRIMINAL"
}
