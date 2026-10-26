// CAIStateGroup_BanditPlayDice::GetName() -- static: the AI state group name "SG_BANDIT_PLAY_DICE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditPlayDice@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE7B0: push esi; push -1; push "SG_BANDIT_PLAY_DICE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122162c registers this GetName with Alloc 0x901c93 (pushed before it); Alloc 0x901c93 stores the vtable of CAIStateGroup_BanditPlayDice into the new object; literal "SG_BANDIT_PLAY_DICE"; PDB name set ?GetName@CAIStateGroup_BanditPlayDice@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditPlayDice@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe7b0::build).
#define CAIStateGroup_BanditPlayDice CAIStateGroup_BanditPlayDiceLayout
#include "engine/CAIStateGroup_BanditPlayDice.h"
#undef CAIStateGroup_BanditPlayDice

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditPlayDice : public CAIStateGroup_BanditPlayDiceLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditPlayDice::GetName()
{
    return CCharString((const char*)0x0128AD3C, -1);   // "SG_BANDIT_PLAY_DICE"
}
