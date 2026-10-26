// CAIStateGroup_StandStill::GetName() -- static: the AI state group name "SG_STAND_STILL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_StandStill@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE08: push esi; push -1; push "SG_STAND_STILL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12207cc registers this GetName with Alloc 0x900460 (pushed before it); Alloc 0x900460 stores the vtable of CAIStateGroup_StandStill into the new object; literal "SG_STAND_STILL"; PDB name set ?GetName@CAIStateGroup_StandStill@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_StandStill@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde08::build).
#define CAIStateGroup_StandStill CAIStateGroup_StandStillLayout
#include "engine/CAIStateGroup_StandStill.h"
#undef CAIStateGroup_StandStill

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_StandStill : public CAIStateGroup_StandStillLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_StandStill::GetName()
{
    return CCharString((const char*)0x01289D74, -1);   // "SG_STAND_STILL"
}
