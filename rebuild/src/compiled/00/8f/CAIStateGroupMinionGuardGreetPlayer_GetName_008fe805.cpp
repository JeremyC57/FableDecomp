// CAIStateGroup_MinionGuardGreetPlayer::GetName() -- static: the AI state group name "SG_MINION_GUARD_GREET_PLAYER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGuardGreetPlayer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE805: push esi; push -1; push "SG_MINION_GUARD_GREET_PLAYER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122167c registers this GetName with Alloc 0x901d13 (pushed before it); Alloc 0x901d13 stores the vtable of CAIStateGroup_MinionGuardGreetPlayer into the new object; literal "SG_MINION_GUARD_GREET_PLAYER"; PDB name set ?GetName@CAIStateGroup_MinionGuardGreetPlayer@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGuardGreetPlayer@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe805::build).
#define CAIStateGroup_MinionGuardGreetPlayer CAIStateGroup_MinionGuardGreetPlayerLayout
#include "engine/CAIStateGroup_MinionGuardGreetPlayer.h"
#undef CAIStateGroup_MinionGuardGreetPlayer

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGuardGreetPlayer : public CAIStateGroup_MinionGuardGreetPlayerLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGuardGreetPlayer::GetName()
{
    return CCharString((const char*)0x0128ADFC, -1);   // "SG_MINION_GUARD_GREET_PLAYER"
}
