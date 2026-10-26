// CAIStateGroup_KidWanderNearKids::GetName() -- static: the AI state group name "SG_KID_WANDER_NEAR_KIDS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KidWanderNearKids@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD979: push esi; push -1; push "SG_KID_WANDER_NEAR_KIDS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122027c registers this GetName with Alloc 0x8ffb64 (pushed before it); Alloc 0x8ffb64 stores the vtable of CAIStateGroup_KidWanderNearKids into the new object; literal "SG_KID_WANDER_NEAR_KIDS"; PDB name set ?GetName@CAIStateGroup_KidWanderNearKids@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KidWanderNearKids@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd979::build).
#define CAIStateGroup_KidWanderNearKids CAIStateGroup_KidWanderNearKidsLayout
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#undef CAIStateGroup_KidWanderNearKids

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KidWanderNearKids : public CAIStateGroup_KidWanderNearKidsLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KidWanderNearKids::GetName()
{
    return CCharString((const char*)0x0128977C, -1);   // "SG_KID_WANDER_NEAR_KIDS"
}
