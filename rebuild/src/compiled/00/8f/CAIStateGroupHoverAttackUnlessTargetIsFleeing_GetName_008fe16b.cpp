// CAIStateGroup_HoverAttackUnlessTargetIsFleeing::GetName() -- static: the AI state group name "SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverAttackUnlessTargetIsFleeing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE16B: push esi; push -1; push "SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220934 registers this GetName with Alloc 0x9029a2 (pushed before it); Alloc 0x9029a2 stores the vtable of CAIStateGroup_HoverAttackUnlessTargetIsFleeing into the new object; literal "SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING"; PDB name set ?GetName@CAIStateGroup_HoverAttackUnlessTargetIsFleeing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverAttackUnlessTargetIsFleeing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe16b::build).
#define CAIStateGroup_HoverAttackUnlessTargetIsFleeing CAIStateGroup_HoverAttackUnlessTargetIsFleeingLayout
#include "engine/CAIStateGroup_HoverAttackUnlessTargetIsFleeing.h"
#undef CAIStateGroup_HoverAttackUnlessTargetIsFleeing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverAttackUnlessTargetIsFleeing : public CAIStateGroup_HoverAttackUnlessTargetIsFleeingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverAttackUnlessTargetIsFleeing::GetName()
{
    return CCharString((const char*)0x0128A278, -1);   // "SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING"
}
