// CAIStateGroup_BanditKingThrowWeapons::GetName() -- static: the AI state group name "SG_BANDIT_KING_THROW_WEAPONS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditKingThrowWeapons@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE405: push esi; push -1; push "SG_BANDIT_KING_THROW_WEAPONS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12211cc registers this GetName with Alloc 0x902b1c (pushed before it); Alloc 0x902b1c -> ctor 0x9012c2 stores (last) the vtable of CAIStateGroup_BanditKingThrowWeapons at this+0; literal "SG_BANDIT_KING_THROW_WEAPONS"; PDB name set ?GetName@CAIStateGroup_BanditKingThrowWeapons@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditKingThrowWeapons@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe405::build).
#define CAIStateGroup_BanditKingThrowWeapons CAIStateGroup_BanditKingThrowWeaponsLayout
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"
#undef CAIStateGroup_BanditKingThrowWeapons

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditKingThrowWeapons : public CAIStateGroup_BanditKingThrowWeaponsLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditKingThrowWeapons::GetName()
{
    return CCharString((const char*)0x0128A754, -1);   // "SG_BANDIT_KING_THROW_WEAPONS"
}
