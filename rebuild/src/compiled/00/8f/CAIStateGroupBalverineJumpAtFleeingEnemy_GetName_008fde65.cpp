// CAIStateGroup_BalverineJumpAtFleeingEnemy::GetName() -- static: the AI state group name "SG_BALVERINE_JUMP_AT_FLEEING_ENEMY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BalverineJumpAtFleeingEnemy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE65: push esi; push -1; push "SG_BALVERINE_JUMP_AT_FLEEING_ENEMY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220844 registers this GetName with Alloc 0x902670 (pushed before it); Alloc 0x902670 -> ctor 0x90054e stores (last) the vtable of CAIStateGroup_BalverineJumpAtFleeingEnemy at this+0; literal "SG_BALVERINE_JUMP_AT_FLEEING_ENEMY"; PDB name set ?GetName@CAIStateGroup_BalverineJumpAtFleeingEnemy@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BalverineJumpAtFleeingEnemy@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde65::build).
#define CAIStateGroup_BalverineJumpAtFleeingEnemy CAIStateGroup_BalverineJumpAtFleeingEnemyLayout
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
#undef CAIStateGroup_BalverineJumpAtFleeingEnemy

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BalverineJumpAtFleeingEnemy : public CAIStateGroup_BalverineJumpAtFleeingEnemyLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BalverineJumpAtFleeingEnemy::GetName()
{
    return CCharString((const char*)0x01289E28, -1);   // "SG_BALVERINE_JUMP_AT_FLEEING_ENEMY"
}
