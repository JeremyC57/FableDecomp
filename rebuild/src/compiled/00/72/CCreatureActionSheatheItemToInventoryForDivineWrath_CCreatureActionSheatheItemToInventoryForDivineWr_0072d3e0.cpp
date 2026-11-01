// CCreatureAction_SheatheItemToInventory_ForDivineWrath::~CCreatureAction_SheatheItemToInventory_ForDivineWrath() -- implicit destructor: release the inherited PFollowUpAction counted pointer (+0xa8), then ~CCreatureActionBase (tail jump)
// Leaf: ??1CCreatureAction_SheatheItemToInventory_ForDivineWrath@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072D3E0, 75 bytes: push esi ; mov esi,ecx ; mov eax,[esi+0ACh] ; test ; je ; dec [eax] ;
//   reload ; cmp [eax],0 ; jne ; mov ecx,[eax+8] ; call [eax+4] ; reload ; push ; call operator delete ; add esp,4 ;
//   mov [esi+0A8h],0 ; mov [esi+0ACh],0 ; mov ecx,esi ; pop esi ; jmp 0x00693EF0.
// Identity (proven): the only caller is CTCSpecialAbilities::SetupDivineWrath 0x0072CD00 (call at 0x0072CDE0), which builds a
//   local CCreatureAction_SheatheItemToInventory_ForDivineWrath (ctor 0x0069F090, Ego_r ??0..@@QAE@AAVCThingCreatureBase@@
//   ABVCCharString@@PAVCCreatureActionBase@@J@Z; vtable 0x0125CA7C) and destroys it by name; the landed caller already binds
//   this address to the class's destructor. FableWin.pdb names ??1CCreatureAction_SheatheItemToInventory_ForDivineWrath@@UAE@XZ
//   (virtual: the root CCreatureActionBase dtor is virtual); Ego_r.pdb has no ??1 for the class (compiler-generated: no
//   vtable store). RTTI ForDivineWrath : CCreatureAction_SheatheItemToInventory : CCreatureActionBase (rtti_bases.tsv). The
//   class's own deleting dtor 0x006A0970 calls ~CCreatureAction_SheatheItemToInventory 0x0048C950 (the landed twin, whose body
//   this inlines). The manifest label `_Dest_val<...CCountedPointer<C3DSkeletalMorphTarget>...>` is propagated.
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC,
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (tail jump).
// Layout: Ego_r.pdb CCreatureAction_SheatheItemToInventory (0xBC) : CCreatureActionBase (0xB4) with PFollowUpAction
//   (CCountedPointer<CCreatureActionBase>) +0xB4; retail CCreatureActionBase is 0xA8 (12-byte STLport vectors), so
//   PFollowUpAction sits at +0xA8; ForDivineWrath adds nothing (Ego_r 0xBC). engine/CCreatureActionBase.h carries the PDB size
//   and a flattened vptr, and engine/CCreatureAction_SheatheItemToInventory_ForDivineWrath.h is a flat vptr + pad record with no
//   base chain, so the chain is modelled locally at its retail size; CCountedPointer is engine/CCountedPointer.h.
// Shape: neither sheathe class declares a destructor; the harness below destroys a local action by name, as SetupDivineWrath
//   does, which emits the implicit ??1 with the base's member release inlined. It is not a retail function.
#include "engine/CCountedPointer.h"
class CThingCreatureBase;
class CCharString;

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {   // vtable 0x0125C83C
public:
    CCountedPointer<CCreatureActionBase> PFollowUpAction;   // +0xA8
};

class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureAction_SheatheItemToInventory {   // vtable 0x0125CA7C
public:
    CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase& creature, const CCharString& itemName,
                                                          CCreatureActionBase* parentAction, long priority);   // 0x0069F090
};

// TU harness (not a retail function): a local sheathe action destroyed by name, as SetupDivineWrath 0x0072CD00 does.
void TUHarness_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase& creature, const CCharString& itemName,
                                                     CCreatureActionBase* parentAction)
{
    CCreatureAction_SheatheItemToInventory_ForDivineWrath action(creature, itemName, parentAction, 0);
}