// CAIStateGroup_CarryCrateAndFollow::GetName() -- static: the AI state group name "SG_CARRY_CRATE_AND_FOLLOW" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CarryCrateAndFollow@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE4FB: push esi; push -1; push "SG_CARRY_CRATE_AND_FOLLOW"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122130c registers this GetName with Alloc 0x902bce (pushed before it); Alloc 0x902bce -> ctor 0x901658 stores (last) the vtable of CAIStateGroup_CarryCrateAndFollow at this+0; literal "SG_CARRY_CRATE_AND_FOLLOW"; PDB name set ?GetName@CAIStateGroup_CarryCrateAndFollow@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CarryCrateAndFollow@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe4fb::build).
#define CAIStateGroup_CarryCrateAndFollow CAIStateGroup_CarryCrateAndFollowLayout
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"
#undef CAIStateGroup_CarryCrateAndFollow

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CarryCrateAndFollow : public CAIStateGroup_CarryCrateAndFollowLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CarryCrateAndFollow::GetName()
{
    return CCharString((const char*)0x0128A8B8, -1);   // "SG_CARRY_CRATE_AND_FOLLOW"
}
