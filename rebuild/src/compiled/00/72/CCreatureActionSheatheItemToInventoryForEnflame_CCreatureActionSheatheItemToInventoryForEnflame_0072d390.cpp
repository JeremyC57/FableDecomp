// CCreatureAction_SheatheItemToInventory_ForEnflame::~CCreatureAction_SheatheItemToInventory_ForEnflame() -- implicit destructor: release the inherited PFollowUpAction counted pointer (+0xA8), then ~CCreatureActionBase (tail jump)
// Leaf: ??1CCreatureAction_SheatheItemToInventory_ForEnflame@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072D390, 75 bytes (int3 padding 0x0072D3DB..DF): push esi ; mov esi,ecx ; the inline
//   ~CCountedPointer on +0xA8 (Info +0xAC: test ; dec [Info] ; reload ; cmp [Info],0 ; jne ; mov ecx,[Info+8] ; call [Info+4] ;
//   delete Info ; then Data = 0, Info = 0) ; mov ecx,esi ; pop esi ; jmp 0x00693EF0. No vtable store: compiler-generated.
// Identity (proven, two sources): (1) the only caller is CTCSpecialAbilities::SetupEnflameSpell 0x0072C790 (call at 0x0072C8E5),
//   which builds a local action at [esp+1Ch] with the ctor 0x0069F060 -- that ctor stores vtable 0x0125C95C, RTTI
//   .?AVCCreatureAction_SheatheItemToInventory_ForEnflame@@ (rtti-vtables.tsv; ??_G slot 0 0x006A0950) -- and destroys it by name;
//   (2) FableWin.pdb ??1CCreatureAction_SheatheItemToInventory_ForEnflame@@UAE@XZ (0x02210A90; virtual: the root CCreatureActionBase dtor
//   is virtual) with its ctors ??0...@@QAE@AAVCThingCreatureBase@@ABVCCharString@@PAVCCreatureActionBase@@J@Z (Ego_r 0x006C3D00, FableWin).
//   It is the byte twin of the landed ~CCreatureAction_SheatheItemToInventory_ForDivineWrath 0x0072D3E0 (next function; caller
//   SetupDivineWrath 0x0072CD00): one class each, two symbols. The landed caller binds this address as
//   ??1CCombatAction_GenericStrikeResponse@@QAE@XZ and the catalog as `_Dest_val<...C3DSkeletalMorphTarget...>`: both wrong (NAME_FIXES.tsv).
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC (??3@YAXPAX@Z),
//   CCreatureActionBase::~CCreatureActionBase 0x00693EF0 (??1CCreatureActionBase@@UAE@XZ, tail jump).
// Layout: RTTI ForEnflame : CCreatureAction_SheatheItemToInventory : CCreatureActionBase. Ego_r CCreatureAction_SheatheItemToInventory
//   (0xBC) = CCreatureActionBase (0xB4) + PFollowUpAction (CCountedPointer<CCreatureActionBase>) +0xB4; retail CCreatureActionBase is
//   0xA8 (12-byte STLport vectors), so PFollowUpAction sits at +0xA8; ForEnflame adds nothing. engine/CCreatureActionBase.h is the
//   flattened PDB-size record, so the base is restated at its retail size with its members as opaque storage (as the landed twin);
//   CCountedPointer is engine/CCountedPointer.h (its own Data-then-Info dtor body).
// Shape: neither sheathe class declares a destructor (no vtable store). The implicit ??1 is emitted where a local action is destroyed
//   by name, as SetupEnflameSpell does; the harness below is that destruction (the landed twin's form). It is not a retail function.
#include "engine/CCountedPointer.h"
class CThingCreatureBase;
class CCharString;

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char CCreatureActionBaseMembers_[0xA8 - 4];    // CCreatureActionBase's own members (opaque, retail +0x04..+0xA8)
};

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {   // vtable 0x0125C83C
public:
    CCountedPointer<CCreatureActionBase> PFollowUpAction;   // +0xA8
};

class CCreatureAction_SheatheItemToInventory_ForEnflame : public CCreatureAction_SheatheItemToInventory {   // vtable 0x0125C95C
public:
    CCreatureAction_SheatheItemToInventory_ForEnflame(CThingCreatureBase& creature, const CCharString& itemName,
                                                      CCreatureActionBase* parentAction, long priority);   // 0x0069F060
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SheatheItemToInventory_ForEnflame) == 0xB0);

// TU harness (not a retail function): a local sheathe action destroyed by name, as SetupEnflameSpell 0x0072C790 does.
void TUHarness_SheatheItemToInventory_ForEnflame(CThingCreatureBase& creature, const CCharString& itemName,
                                                 CCreatureActionBase* parentAction)
{
    CCreatureAction_SheatheItemToInventory_ForEnflame action(creature, itemName, parentAction, 0);
}