// CAIStateGroup_MinionKidCalledToMother::GetName() -- static: the AI state group name "SG_MINION_KID_CALLED_TO_MOTHER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionKidCalledToMother@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD446: push esi; push -1; push "SG_MINION_KID_CALLED_TO_MOTHER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fe1c registers this GetName with Alloc 0x8fef4b (pushed before it); Alloc 0x8fef4b stores the vtable of CAIStateGroup_MinionKidCalledToMother into the new object; literal "SG_MINION_KID_CALLED_TO_MOTHER"; PDB name set ?GetName@CAIStateGroup_MinionKidCalledToMother@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionKidCalledToMother@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd446::build).
#define CAIStateGroup_MinionKidCalledToMother CAIStateGroup_MinionKidCalledToMotherLayout
#include "engine/CAIStateGroup_MinionKidCalledToMother.h"
#undef CAIStateGroup_MinionKidCalledToMother

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionKidCalledToMother : public CAIStateGroup_MinionKidCalledToMotherLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionKidCalledToMother::GetName()
{
    return CCharString((const char*)0x01288F24, -1);   // "SG_MINION_KID_CALLED_TO_MOTHER"
}
