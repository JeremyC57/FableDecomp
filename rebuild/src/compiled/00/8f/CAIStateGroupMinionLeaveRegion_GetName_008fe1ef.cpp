// CAIStateGroup_MinionLeaveRegion::GetName() -- static: the AI state group name "SG_MINION_LEAVE_REGION" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionLeaveRegion@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE1EF: push esi; push -1; push "SG_MINION_LEAVE_REGION"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122108c registers this GetName with Alloc 0x900d9e (pushed before it); Alloc 0x900d9e stores the vtable of CAIStateGroup_MinionLeaveRegion into the new object; literal "SG_MINION_LEAVE_REGION"; PDB name set ?GetName@CAIStateGroup_MinionLeaveRegion@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionLeaveRegion@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe1ef::build).
#define CAIStateGroup_MinionLeaveRegion CAIStateGroup_MinionLeaveRegionLayout
#include "engine/CAIStateGroup_MinionLeaveRegion.h"
#undef CAIStateGroup_MinionLeaveRegion

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionLeaveRegion : public CAIStateGroup_MinionLeaveRegionLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionLeaveRegion::GetName()
{
    return CCharString((const char*)0x0128A364, -1);   // "SG_MINION_LEAVE_REGION"
}
