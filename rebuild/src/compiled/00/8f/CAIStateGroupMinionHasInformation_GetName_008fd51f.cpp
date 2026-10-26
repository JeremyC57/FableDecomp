// CAIStateGroup_MinionHasInformation::GetName() -- static: the AI state group name "SG_MINION_HAS_INFORMATION" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionHasInformation@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD51F: push esi; push -1; push "SG_MINION_HAS_INFORMATION"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fbc4 registers this GetName with Alloc 0x8ff1b8 (pushed before it); Alloc 0x8ff1b8 stores the vtable of CAIStateGroup_MinionHasInformation into the new object; literal "SG_MINION_HAS_INFORMATION"; PDB name set ?GetName@CAIStateGroup_MinionHasInformation@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionHasInformation@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd51f::build).
#define CAIStateGroup_MinionHasInformation CAIStateGroup_MinionHasInformationLayout
#include "engine/CAIStateGroup_MinionHasInformation.h"
#undef CAIStateGroup_MinionHasInformation

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionHasInformation : public CAIStateGroup_MinionHasInformationLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionHasInformation::GetName()
{
    return CCharString((const char*)0x012890A8, -1);   // "SG_MINION_HAS_INFORMATION"
}
