// CCreatureAction_SheatheTrophy::~CCreatureAction_SheatheTrophy() -- implicit destructor: release the inherited PFollowUpAction counted pointer (+0xa8), then ~CCreatureActionBase (tail jump)
// Leaf: ??1CCreatureAction_SheatheTrophy@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DF920, 75 bytes: push esi ; mov esi,ecx ; mov eax,[esi+0ACh] ; test ; je ; dec [eax] ;
//   reload ; cmp [eax],0 ; jne ; mov ecx,[eax+8] ; call [eax+4] ; reload ; push ; call operator delete ; add esp,4 ;
//   mov [esi+0A8h],0 ; mov [esi+0ACh],0 ; mov ecx,esi ; pop esi ; jmp 0x00693EF0. No caller (unreferenced COMDAT).
// Identity (proven): it sits in the CTCTrophy TU between CCreatureAction_SheatheTrophy's constructor 0x006DF900 (calls the
//   CCreatureAction_SheatheItemToInventory(CThingCreatureBase&, CCreatureActionBase*, long) ctor 0x0069FFD0 with (creature, 0,
//   0x3EA), stores vtable 0x0125FB14 = RTTI .?AVCCreatureAction_SheatheTrophy@@) and that class's scalar deleting destructor
//   0x006DF9B0 (slot 0 of 0x0125FB14), which inlines this exact member sequence before `call 0x00693EF0`. The TU destroys a
//   local CCreatureAction_SheatheTrophy by name in CCreatureAction_ShowToSheatheTrophy::FinishAction 0x006DD980 (inline ctor,
//   `mov [esp+8],0x0125FB14`, SetCurrentAction). FableWin.pdb names ??0CCreatureAction_SheatheTrophy@@QAE@AAVCThingCreatureBase@@@Z
//   and ??1CCreatureAction_SheatheTrophy@@UAE@XZ; Ego_r.pdb lists its Clone but no ??1 (compiler-generated: no vtable store).
//   RTTI SheatheTrophy : CCreatureAction_SheatheItemToInventory : CCreatureActionBase (rtti_bases.tsv). The manifest label
//   `_Dest_val<...CCountedPointer<C3DSkeletalMorphTarget>...>` is propagated.
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC,
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (tail jump).
// Layout: Ego_r.pdb CCreatureAction_SheatheItemToInventory (0xBC) : CCreatureActionBase (0xB4) with PFollowUpAction
//   (CCountedPointer<CCreatureActionBase>) +0xB4; retail CCreatureActionBase is 0xA8, so PFollowUpAction sits at +0xA8;
//   SheatheTrophy adds nothing. engine/CCreatureActionBase.h carries the PDB size and a flattened vptr, so the base is
//   modelled locally at its retail size; CCountedPointer is engine/CCountedPointer.h.
// Shape: no destructor declared at any sheathe level; the constructor is inline (retail expands it at 0x006DD9A4 and keeps
//   a COMDAT at 0x006DF900); the harness below destroys a local action by name, as FinishAction does, which emits the
//   implicit ??1. It is not a retail function.
#include "engine/CCountedPointer.h"
class CThingCreatureBase;

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {   // vtable 0x0125C83C
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, CCreatureActionBase* parentAction,
                                           long priority);  // 0x0069FFD0
    CCountedPointer<CCreatureActionBase> PFollowUpAction;   // +0xA8
};

class CCreatureAction_SheatheTrophy : public CCreatureAction_SheatheItemToInventory {   // vtable 0x0125FB14
public:
    CCreatureAction_SheatheTrophy(CThingCreatureBase& creature)
        : CCreatureAction_SheatheItemToInventory(creature, 0, 0x3ea)
    {
    }
};

// TU harness (not a retail function): a local sheathe action destroyed by name, as ShowToSheatheTrophy::FinishAction does.
void TUHarness_SheatheTrophy(CThingCreatureBase& creature)
{
    CCreatureAction_SheatheTrophy action(creature);
}