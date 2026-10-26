// CAIStateGroup_MinionGuardReactToBribe::GetName() -- static: the AI state group name "SG_MINION_GUARD_REACT_TO_BRIBE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardReactToBribe@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD369: push esi; push -1; push "SG_MINION_GUARD_REACT_TO_BRIBE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fa5c registers this GetName with Alloc 0x901f97 (pushed before it); Alloc 0x901f97 -> ctor 0x8fed51 stores (last) the vtable of CAIStateGroup_MinionGuardReactToBribe at this+0; literal "SG_MINION_GUARD_REACT_TO_BRIBE"; PDB name set ?GetName@CAIStateGroup_MinionGuardReactToBribe@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardReactToBribe@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd369::build).
#define CAIStateGroup_MinionGuardReactToBribe CAIStateGroup_MinionGuardReactToBribeLayout
#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"
#undef CAIStateGroup_MinionGuardReactToBribe

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardReactToBribe : public CAIStateGroup_MinionGuardReactToBribeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardReactToBribe::GetName()
{
    return CCharString((const char*)0x01288D88, -1);   // "SG_MINION_GUARD_REACT_TO_BRIBE"
}
