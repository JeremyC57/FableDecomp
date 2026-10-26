// CAIStateGroup_BanditKingLunge::GetName() -- static: the AI state group name "SG_BANDIT_KING_LUNGE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditKingLunge@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE3F2: push esi; push -1; push "SG_BANDIT_KING_LUNGE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12211a4 registers this GetName with Alloc 0x902b06 (pushed before it); Alloc 0x902b06 -> ctor 0x901274 stores (last) the vtable of CAIStateGroup_BanditKingLunge at this+0; literal "SG_BANDIT_KING_LUNGE"; PDB name set ?GetName@CAIStateGroup_BanditKingLunge@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditKingLunge@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe3f2::build).
#define CAIStateGroup_BanditKingLunge CAIStateGroup_BanditKingLungeLayout
#include "engine/CAIStateGroup_BanditKingLunge.h"
#undef CAIStateGroup_BanditKingLunge

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditKingLunge : public CAIStateGroup_BanditKingLungeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditKingLunge::GetName()
{
    return CCharString((const char*)0x0128A73C, -1);   // "SG_BANDIT_KING_LUNGE"
}
