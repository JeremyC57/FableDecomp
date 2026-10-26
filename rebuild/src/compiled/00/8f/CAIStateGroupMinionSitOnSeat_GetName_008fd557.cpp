// CAIStateGroup_MinionSitOnSeat::GetName() -- static: the AI state group name "SG_MINION_SIT_ON_SEAT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionSitOnSeat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD557: push esi; push -1; push "SG_MINION_SIT_ON_SEAT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fbec registers this GetName with Alloc 0x902d71 (pushed before it); Alloc 0x902d71 stores the vtable of CAIStateGroup_MinionSitOnSeat into the new object; literal "SG_MINION_SIT_ON_SEAT"; PDB name set ?GetName@CAIStateGroup_MinionSitOnSeat@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionSitOnSeat@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd557::build).
#define CAIStateGroup_MinionSitOnSeat CAIStateGroup_MinionSitOnSeatLayout
#include "engine/CAIStateGroup_MinionSitOnSeat.h"
#undef CAIStateGroup_MinionSitOnSeat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionSitOnSeat : public CAIStateGroup_MinionSitOnSeatLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionSitOnSeat::GetName()
{
    return CCharString((const char*)0x01289100, -1);   // "SG_MINION_SIT_ON_SEAT"
}
