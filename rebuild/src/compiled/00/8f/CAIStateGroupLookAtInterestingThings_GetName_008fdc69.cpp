// CAIStateGroup_LookAtInterestingThings::GetName() -- static: the AI state group name "SG_LOOK_AT_INTERESTING_THINGS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_LookAtInterestingThings@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDC69: push esi; push -1; push "SG_LOOK_AT_INTERESTING_THINGS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122077c registers this GetName with Alloc 0x90007c (pushed before it); Alloc 0x90007c stores the vtable of CAIStateGroup_LookAtInterestingThings into the new object; literal "SG_LOOK_AT_INTERESTING_THINGS"; PDB name set ?GetName@CAIStateGroup_LookAtInterestingThings@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_LookAtInterestingThings@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdc69::build).
#define CAIStateGroup_LookAtInterestingThings CAIStateGroup_LookAtInterestingThingsLayout
#include "engine/CAIStateGroup_LookAtInterestingThings.h"
#undef CAIStateGroup_LookAtInterestingThings

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_LookAtInterestingThings : public CAIStateGroup_LookAtInterestingThingsLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_LookAtInterestingThings::GetName()
{
    return CCharString((const char*)0x01289B98, -1);   // "SG_LOOK_AT_INTERESTING_THINGS"
}
