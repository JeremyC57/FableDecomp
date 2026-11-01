// CCreatureAction_JackScream::~CCreatureAction_JackScream() -- implicit destructor: release the PModeCountedPointer counted pointer (+0x114), then ~CCombatActionBase (tail jump)
// Leaf: ??1CCreatureAction_JackScream@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009771A0, 75 bytes: push esi ; mov esi,ecx ; mov eax,[esi+118h] ; test ; je ; dec [eax] ;
//   reload ; cmp [eax],0 ; jne ; mov ecx,[eax+8] ; call [eax+4] ; reload ; push ; call operator delete ; add esp,4 ;
//   mov [esi+114h],0 ; mov [esi+118h],0 ; mov ecx,esi ; pop esi ; jmp 0x0062E900. No caller (unreferenced COMDAT).
// Identity (proven): it sits in the Jack of Blades action TU right after CCreatureAction_JackScream's constructor 0x00977100
//   (stores vtable 0x01297CB4 = RTTI .?AVCCreatureAction_JackScream@@, zeroes +0x114/+0x118) and its IsToEvadeAllHits
//   0x00977190 (slot of 0x01297CB4); that vtable's slot 0, the scalar deleting destructor 0x00977440, inlines this exact
//   sequence before `call 0x0062E900`, and the TU destroys a temporary JackScream by name after constructing it at 0x00975758
//   (the same release inlined at 0x00975765). FableWin.pdb names ??1CCreatureAction_JackScream@@UAE@XZ; Ego_r.pdb has the
//   class's ctor, ??_E and Clone but no ??1 (compiler-generated: no vtable store). RTTI JackScream : CCombatActionBase :
//   CCreatureActionBase (rtti_bases.tsv). The manifest label `_Dest_val<...CCountedPointer<C3DSkeletalMorphTarget>...>` is
//   propagated.
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC,
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (landed; tail jump).
// Layout: Ego_r.pdb CCreatureAction_JackScream (0x134) has PModeCountedPointer
//   (CCountedPointer<NCreatureMode::CModeJackOfBladesScream>) at +0x12C; retail CCombatActionBase is 0x114 (landed
//   0x0062E900: retail CCreatureActionBase 0xA8, STLport containers), so it sits at +0x114 (the ctor zeroes +0x114/+0x118).
//   engine/CCreatureAction_JackScream.h has the retail offsets (+0x114/+0x118) but flattens the vptr and the whole base chain
//   into pads (no CCombatActionBase destructor to chain to), and engine/CCreatureActionBase.h carries the PDB size, so the
//   chain is modelled locally at retail size; CCountedPointer is engine/CCountedPointer.h.
// Shape: the class declares no destructor; the harness below destroys a local action by name, as the TU's user at
//   0x00975758 does, which emits the implicit ??1. It is not a retail function.
#include "engine/CCountedPointer.h"
class CThingCreatureBase;
namespace NCreatureMode { class CModeJackOfBladesScream; }

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCombatActionBase : public CCreatureActionBase {      // retail sizeof 0x114
public:
    virtual ~CCombatActionBase();                           // 0x0062E900
private:
    unsigned char _members[0x114 - 0xa8];                   // PTarget .. PreferredMovementVector (landed 0x0062E900)
};

class CCreatureAction_JackScream : public CCombatActionBase {   // vtable 0x01297CB4
public:
    CCreatureAction_JackScream(CThingCreatureBase& creature);   // 0x00977100 (Ego_r ??0CCreatureAction_JackScream@@QAE@AAVCThingCreatureBase@@@Z)
    CCountedPointer<NCreatureMode::CModeJackOfBladesScream> PModeCountedPointer;   // +0x114
};

// TU harness (not a retail function): a local scream action destroyed by name, as the TU's user at 0x00975758 does.
void TUHarness_JackScream(CThingCreatureBase& creature)
{
    CCreatureAction_JackScream action(creature);
}