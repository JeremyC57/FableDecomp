// CAIStateGroup_KidHurtSelfRunToMother::GetName() -- static: the AI state group name "SG_KID_HURT_SELF_RUN_TO_MOTHER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KidHurtSelfRunToMother@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD9C7: push esi; push -1; push "SG_KID_HURT_SELF_RUN_TO_MOTHER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12202f4 registers this GetName with Alloc 0x8ffc38 (pushed before it); Alloc 0x8ffc38 stores the vtable of CAIStateGroup_KidHurtSelfRunToMother into the new object; literal "SG_KID_HURT_SELF_RUN_TO_MOTHER"; PDB name set ?GetName@CAIStateGroup_KidHurtSelfRunToMother@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KidHurtSelfRunToMother@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd9c7::build).
#define CAIStateGroup_KidHurtSelfRunToMother CAIStateGroup_KidHurtSelfRunToMotherLayout
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#undef CAIStateGroup_KidHurtSelfRunToMother

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KidHurtSelfRunToMother : public CAIStateGroup_KidHurtSelfRunToMotherLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KidHurtSelfRunToMother::GetName()
{
    return CCharString((const char*)0x01289800, -1);   // "SG_KID_HURT_SELF_RUN_TO_MOTHER"
}
