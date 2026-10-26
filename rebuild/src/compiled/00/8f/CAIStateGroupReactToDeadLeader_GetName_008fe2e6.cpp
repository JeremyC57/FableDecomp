// CAIStateGroup_ReactToDeadLeader::GetName() -- static: the AI state group name "SG_REACT_TO_DEAD_LEADER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ReactToDeadLeader@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE2E6: push esi; push -1; push "SG_REACT_TO_DEAD_LEADER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12210b4 registers this GetName with Alloc 0x902a74 (pushed before it); Alloc 0x902a74 -> ctor 0x900fac stores (last) the vtable of CAIStateGroup_ReactToDeadLeader at this+0; literal "SG_REACT_TO_DEAD_LEADER"; PDB name set ?GetName@CAIStateGroup_ReactToDeadLeader@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ReactToDeadLeader@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe2e6::build).
#define CAIStateGroup_ReactToDeadLeader CAIStateGroup_ReactToDeadLeaderLayout
#include "engine/CAIStateGroup_ReactToDeadLeader.h"
#undef CAIStateGroup_ReactToDeadLeader

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ReactToDeadLeader : public CAIStateGroup_ReactToDeadLeaderLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ReactToDeadLeader::GetName()
{
    return CCharString((const char*)0x0128A5A0, -1);   // "SG_REACT_TO_DEAD_LEADER"
}
