// CAIStateGroup_WorkAtSchool::GetName() -- static: the AI state group name "SG_WORK_AT_SCHOOL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WorkAtSchool@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD9FF: push esi; push -1; push "SG_WORK_AT_SCHOOL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122036c registers this GetName with Alloc 0x902466 (pushed before it); Alloc 0x902466 stores the vtable of CAIStateGroup_WorkAtSchool into the new object; literal "SG_WORK_AT_SCHOOL"; PDB name set ?GetName@CAIStateGroup_WorkAtSchool@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WorkAtSchool@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd9ff::build).
#define CAIStateGroup_WorkAtSchool CAIStateGroup_WorkAtSchoolLayout
#include "engine/CAIStateGroup_WorkAtSchool.h"
#undef CAIStateGroup_WorkAtSchool

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WorkAtSchool : public CAIStateGroup_WorkAtSchoolLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WorkAtSchool::GetName()
{
    return CCharString((const char*)0x01289878, -1);   // "SG_WORK_AT_SCHOOL"
}
