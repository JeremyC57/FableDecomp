// CCombatSequenceGeneric::OnCreateSpecial(const std::map<CCharString, creator>& specials) -- fill a generic combat sequence from its def: a def with an empty ActionList names a special sequence (its instantiation name), which is created through the registered creator, given OnCreate and copied with CloneActions (CombatActions, NoAvailableActions); otherwise one combat action is made per EActionRegisteredType in the def's ActionList and pushed; then CreateCalled is set
// Leaf: ?OnCreateSpecial@CCombatSequenceGeneric@@UAEXABV?$map@VCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@ZU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@Z@std@@@5@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0B60, 432 bytes, thiscall, `ret 4`; vtable 0x01261D74 slot 19 (the only reference).
// Name: Ego_r.pdb and FableWin.pdb both carry the Leaf symbol (public virtual, one const map& argument).
// Callees: def vslot 10 CDefClassBase::GetInstantiationName (+0x28); CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828
//   (CDefString::GetString inline); _Rb_tree::_M_find 0x006F1360 (map::find inline, node compared with the header); the map's creator
//   (__fastcall, ecx = *Creature, edx = *Target); CCountedPointer<CCombatSequenceBase>::CCountedPointer(CCombatSequenceBase*) 0x006EF6F0;
//   sequence vslot 1 OnCreate (+4); std::vector<CCombatAction>::operator= 0x008B3180; CCharString::~CCharString 0x0099EAE0;
//   CCombatActionBase::GetPCombatActionFromEnum 0x00857430 (static __fastcall, hidden result); vector<CCombatAction>::_M_insert_overflow
//   0x008B3350; CCPPointerInfo::DeleteFunc (indirect) and operator delete 0x00BFE9BC in the three inline counted-pointer releases.
// Shape: `and esp,-8` frame; the special path is the fall-through. `Def->GetInstantiationName().GetString()` builds the name straight
//   in its slot (the CDefString -> CCharString conversion operator costs a copy + destructor instead). The creator is read into a local
//   before its arguments (`mov eax,[eax+14h]` ... `call eax`), and the enum is read before the creature/target (`mov edx,[eax]` first).
//   The counted-pointer constructor is called out of line but its body is visible (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR), so
//   the sequence pointer and its count block stay in edi across the calls and the release uses the `dec` flags. The loop tail is
//   CCombatSequenceBase::PushAction 0x008B0B10 (defined just before this function in the same retail source file, real landed body
//   below) expanded inline: push_back + ++NoAvailableActions. The special path's copy is the protected helper
//   CCombatSequenceBase::CloneActions (Ego_r ?CloneActions@CCombatSequenceBase@@IAEXABV1@@Z 0x008EC0D0, between PushAction and
//   OnCreateSpecial; FableWin 0x02A37EB0); Fable.exe has no out-of-line body for it (PushAction ends 0x008B0B5F, this starts
//   0x008B0B60), so it was inlined here: vector operator= 0x008B3180 + the NoAvailableActions copy. The loop re-reads Def->ActionList's end each iteration.
// Layout: retail CCombatSequenceBase (no shared header): Running +4, NoAvailableActions +8, CurrentActionIndex +0xC, CombatActions +0x10
//   (STLport vector, 12 bytes), Creature +0x1C, Target +0x20, CreateCalled +0x24; CCombatSequenceGeneric::Def +0x28;
//   CCombatSequenceDef::ActionList +0x54 (Ego_r +0x58); CCombatAction = { CCountedPointer<CCombatActionBase> CreatureAction } (8 bytes).
// Headers: hdr overlay opt-ins FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_ONCREATESPECIAL (engine/CCombatSequenceGeneric.h) and
//   FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM (engine/CCombatActionBase.h). No extra compiler flags.
// Descriptive (not PDB) names: the typedef TSequenceCreator, the parameter `specials`, the locals.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"

class CThingCreatureBase;
#include "engine/EActionRegisteredType.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM
#include "engine/CCombatActionBase.h"                       // GetPCombatActionFromEnum 0x00857430

class CCombatAction {
public:
    CCountedPointer<CCombatActionBase> CreatureAction;      // +0x00
};

class CCombatSequenceDef : public CDefClassBase {
public:
    unsigned char _pad[0x54 - sizeof(CDefClassBase)];
    std::vector<EActionRegisteredType> ActionList;          // +0x54
};

class CCombatSequenceBase {
public:
    virtual ~CCombatSequenceBase();                         // slot 0
    virtual void OnCreate();                                // slot 1
    bool                          Running;                  // +0x04
    long                          NoAvailableActions;       // +0x08
    long                          CurrentActionIndex;       // +0x0C
    std::vector<CCombatAction>    CombatActions;            // +0x10
    CThingCreatureBase*           Creature;                 // +0x1C
    CThingCreatureBase*           Target;                   // +0x20
    bool                          CreateCalled;             // +0x24
protected:
    void PushAction(CCombatAction& action);                 // 0x008B0B10
    void CloneActions(const CCombatSequenceBase& other);    // Ego_r 0x008EC0D0 (inlined in Fable.exe)
};

#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC
#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_ONCREATESPECIAL
#include "engine/CCombatSequenceGeneric.h"

typedef CCombatSequenceBase* (__fastcall* TSequenceCreator)(CThingCreatureBase&, CThingCreatureBase&);

void CCombatSequenceBase::PushAction(CCombatAction& action)
{
    CombatActions.push_back(action);
    ++NoAvailableActions;
}

inline void CCombatSequenceBase::CloneActions(const CCombatSequenceBase& other)
{
    CombatActions = other.CombatActions;
    NoAvailableActions = other.NoAvailableActions;
}


void CCombatSequenceGeneric::OnCreateSpecial(const std::map<CCharString, TSequenceCreator>& specials)
{
    if (Def->ActionList.empty()) {
        CCharString name = Def->GetInstantiationName().GetString();
        std::map<CCharString, TSequenceCreator>::const_iterator found = specials.find(name);
        if (found != specials.end()) {
            TSequenceCreator creator = (*found).second;
            CCountedPointer<CCombatSequenceBase> sequence(creator(*Creature, *Target));
            sequence->OnCreate();
            CloneActions(*sequence);
        }
    }
    else {
        for (std::vector<EActionRegisteredType>::const_iterator it = Def->ActionList.begin(); it != Def->ActionList.end(); ++it) {
            CCombatAction action;
            EActionRegisteredType type = *it;
            action.CreatureAction = CCombatActionBase::GetPCombatActionFromEnum(type, *Creature, *Target);
            PushAction(action);
        }
    }
    CreateCalled = true;
}