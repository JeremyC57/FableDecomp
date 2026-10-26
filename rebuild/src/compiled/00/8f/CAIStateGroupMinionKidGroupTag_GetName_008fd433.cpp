// CAIStateGroup_MinionKidGroupTag::GetName() -- static: the AI state group name "SG_MINION_KID_GROUP_TAG" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionKidGroupTag@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD433: push esi; push -1; push "SG_MINION_KID_GROUP_TAG"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fdf4 registers this GetName with Alloc 0x901ffd (pushed before it); Alloc 0x901ffd stores the vtable of CAIStateGroup_MinionKidGroupTag into the new object; literal "SG_MINION_KID_GROUP_TAG"; PDB name set ?GetName@CAIStateGroup_MinionKidGroupTag@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionKidGroupTag@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd433::build).
#define CAIStateGroup_MinionKidGroupTag CAIStateGroup_MinionKidGroupTagLayout
#include "engine/CAIStateGroup_MinionKidGroupTag.h"
#undef CAIStateGroup_MinionKidGroupTag

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionKidGroupTag : public CAIStateGroup_MinionKidGroupTagLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionKidGroupTag::GetName()
{
    return CCharString((const char*)0x01288F0C, -1);   // "SG_MINION_KID_GROUP_TAG"
}
