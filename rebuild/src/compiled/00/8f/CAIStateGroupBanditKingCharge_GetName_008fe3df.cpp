// CAIStateGroup_BanditKingCharge::GetName() -- static: the AI state group name "SG_BANDIT_KING_CHARGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditKingCharge@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE3DF: push esi; push -1; push "SG_BANDIT_KING_CHARGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122117c registers this GetName with Alloc 0x902af0 (pushed before it); Alloc 0x902af0 -> ctor 0x901226 stores (last) the vtable of CAIStateGroup_BanditKingCharge at this+0; literal "SG_BANDIT_KING_CHARGE"; PDB name set ?GetName@CAIStateGroup_BanditKingCharge@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditKingCharge@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe3df::build).
#define CAIStateGroup_BanditKingCharge CAIStateGroup_BanditKingChargeLayout
#include "engine/CAIStateGroup_BanditKingCharge.h"
#undef CAIStateGroup_BanditKingCharge

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditKingCharge : public CAIStateGroup_BanditKingChargeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditKingCharge::GetName()
{
    return CCharString((const char*)0x0128A724, -1);   // "SG_BANDIT_KING_CHARGE"
}
