// CAIStateGroup_BalverineCornered::GetName() -- static: the AI state group name "SG_BALVERINE_CORNERED" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BalverineCornered@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE52: push esi; push -1; push "SG_BALVERINE_CORNERED"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122081c registers this GetName with Alloc 0x90265a (pushed before it); Alloc 0x90265a -> ctor 0x9004e0 stores (last) the vtable of CAIStateGroup_BalverineCornered at this+0; literal "SG_BALVERINE_CORNERED"; PDB name set ?GetName@CAIStateGroup_BalverineCornered@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BalverineCornered@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde52::build).
#define CAIStateGroup_BalverineCornered CAIStateGroup_BalverineCorneredLayout
#include "engine/CAIStateGroup_BalverineCornered.h"
#undef CAIStateGroup_BalverineCornered

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BalverineCornered : public CAIStateGroup_BalverineCorneredLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BalverineCornered::GetName()
{
    return CCharString((const char*)0x01289E10, -1);   // "SG_BALVERINE_CORNERED"
}
