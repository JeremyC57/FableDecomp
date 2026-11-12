// CTCKraken::CreateTentacles() -- unless the editor is active, spawn one tentacle creature (the current phase's tentacle def) per
// kraken carry slot: each is linked back to the kraken, kept out of the save, made non-targetable and undrawable, faced along its
// slot's angle at the slot's dummy, and appended to the tentacle list; the kraken itself is made non-targetable and the
// attacking-tentacle cursor is reset to the list's end.
// Leaf: ?CreateTentacles@CTCKraken@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FC210, 1245 bytes, plain `ret`; 8-aligned ebp frame.
// Callees (Ego_r.pdb / FableWin.pdb spellings): CTCBase::PeekComponent 0x00686D60 + CMainGameComponent::IsEditorActive (slot 9),
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410 / GetDef<CCarrySlotDef>
//   0x006A3060, vector<pair<long,float> >::_M_insert_overflow 0x006B5BE0, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CThing::GetPos 0x004C73D0, CThingAICreature::Create 0x00833800 (__fastcall; the init a default-constructed temporary),
//   CVectorMap LowerBound 0x0040F020, CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::SetAsGameStatePersisted 0x004C7880, CDefStringTable::GetString
//   0x009D49B0, CTCKrakenTentacle::SetFacingAngleAndDummy 0x007F15D0, CThing::SetAsDrawable (slot 28, +0x70), malloc 0x00BFEA0E.
// Layout: engine/CTCKraken.h (PKrakenDef, Tentacles, AttackingTentacleIterator, Phase), engine/CKrakenDef.h (TentacleDefIndex),
//   engine/CTCTargeted.h (SetTargetable, +0x10). CTCKrakenTentacle and CCarrySlotDef have no shared header: local models with their
//   Ego_r.pdb members (CTCKrakenTentacle : CTCBase, PTentacleDef +0x0C, PKraken +0x10; CCarrySlotDef : CSubDefClassBase,
//   DummyPosName +0x28).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CDefString.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CKrakenDef.h"
#include "engine/CTCKraken.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"

class CKrakenTentacleDef;
class CTCKrakenTentacle : public CTCBase {                   // interface TCI_KRAKEN_TENTACLE (227); Ego_r.pdb members
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_KRAKEN_TENTACLE; }
    void SetFacingAngleAndDummy(float facing_angle, const CCharString& dummy_name);   // 0x007F15D0
    CDefPointer<const CKrakenTentacleDef> PTentacleDef;         // +0x0c
    CIntelligentPointer<CThing>           PKraken;              // +0x10
};
class CCarrySlotDef : public CSubDefClassBase {              // RTTI : CSubDefClassBase; Ego_r.pdb members
public:
    CDefString DummyPosName;                                 // +0x28
    long       DummyPosIndex;                                // +0x2c
    bool       PrimarySlot;                                  // +0x30
    CDefString SecondaryDummyPosName;                        // +0x34
};

void CTCKraken::CreateTentacles()
{
    if (PeekComponent().IsEditorActive())
        return;
    const CGameDefinitionManager& definitions = GetDefinitionManager();
    std::vector<std::pair<long, float> > slots;
    slots.push_back(std::pair<long, float>(definitions.GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_01"), 0.375f));
    slots.push_back(std::pair<long, float>(definitions.GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_02"), 0.625f));
    slots.push_back(std::pair<long, float>(definitions.GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_03"), 0.875f));
    slots.push_back(std::pair<long, float>(definitions.GetDefGlobalIndexFromName("CARRY_SLOT_KRAKEN_TENTACLE_04"), 0.125f));
    long tentacle_def = PKrakenDef->TentacleDefIndex[(PKrakenDef->TentacleDefIndex.size() - 1 < (unsigned long)Phase) ?
                                                     PKrakenDef->TentacleDefIndex.size() - 1 : (unsigned long)Phase];
    CTCKrakenTentacle* ptc_tentacle;
    CTCTargeted* ptc_targeted;
    for (unsigned long i = 0; i < slots.size(); ++i) {
        CThingAICreature* tentacle = CThingAICreature::Create(tentacle_def, GetParentThing().GetPos(), GetParentThing().GetPlayerNumber(), CThingAICreatureInit());
        tentacle->GetTC(&ptc_tentacle);
        ptc_tentacle->PKraken = CIntelligentPointer<CThing>(&GetParentThing());
        tentacle->SetAsGameStatePersisted(false);
        tentacle->SetAsCreatedByOwnerThingCreation(true);
        tentacle->SetLeaveExperienceOrbsOverride(false);
        tentacle->GetTC(&ptc_targeted);
        ptc_targeted->SetTargetable(false);
        CDefPointer<const CCarrySlotDef> slot_def;
        definitions.GetDef(slots[i].first, &slot_def);
        ptc_tentacle->SetFacingAngleAndDummy(slots[i].second, slot_def->DummyPosName.GetString());
        tentacle->SetAsDrawable(false);
        Tentacles.push_back(CIntelligentPointer<CThing>(tentacle));
    }
    GetParentThing().GetTC(&ptc_targeted);
    ptc_targeted->SetTargetable(false);
    AttackingTentacleIterator = Tentacles.end();
}