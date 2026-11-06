// CThingObject::OnHit(CHitParameters& params) -- after CThingGameObject::OnHit: a melee strike that is directional and localised is
//   forwarded to the shot-definition component (OnHitByMelee); a destroyed object hit by a thing carrying a hero opinion deed log posts a
//   vandalism deed (village member / door that posts vandalism / owned by a village member; buyable-house owner -> incomprehension, shop
//   door -> alarm + shopkeeper witness) and docks morality, else a "BARREL" def posts vandalism or incomprehension; a damageable object
//   that is not a door the hitter cannot break down, nor a door of a village in limbo, gets a CTCWobble component.
// Leaf: ?OnHit@CThingObject@@MAEXAAVCHitParameters@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007039A0, 1848 bytes (protected virtual, `ret 4`).
// Callees: CThingGameObject::OnHit 0x00839940, CBaseIntelligentPointer::GetPItem 0x00A01B50, CHitParameters::IsMeleeStrike 0x00868860,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 / const 0x004365B0, NThingShot::CTCShotDefinitionBase::OnHitByMelee
//   (vtable slot 29, [eax+74h]), CTCVillageMember::GetPVillage 0x007EE540, CTCDoor::PostVandalismCrimeIfDestroyed 0x007305F0,
//   IS_THING_ALIVE<CThing*> 0x0040DCE0, IS_THING_ALIVE<CThingCreatureBase*> 0x005C05C0, CThing::GetPos 0x004C73D0,
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CTCShop::SetAlarm 0x00773C90, CThing::PeekDefinitionManager 0x004C7990,
//   CTCHeroStats::SetMorality 0x0057C2E6, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CThing::GetDefName
//   0x004C7CC0, CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828, strstr (MSVCR71 0x00BFEB3A),
//   CTCHeroExperience::IsAbleToBreakDownDoors 0x006D6370, CThing::AddTC 0x004C9D60.
// Names: Ego_r.pdb / FableWin.pdb ?OnHit@CThingObject@@MAEXAAVCHitParameters@@@Z; callee spellings in the overlay headers (hdr/README.md).
// Shape: the "posted" flag is what retail's `xor bl,bl` (0x00703C87) and the flag-guarded BARREL / GetString temporaries show: the barrel
//   test is `!posted && name.Contains("BARREL")` (one full expression, flags word at [esp+10h]); Contains("BARREL") (implicit conversion,
//   pattern built first) with GetString forced inline gives retail's order; the morality add reads Morality directly (public member).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONTAINS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGOBJECT_ONHIT
#define FABLE_RETAIL_CHITPARAMETERS_BLOCKABLE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_SHOPKEEPER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BREAKDOORS
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_VANDALISM
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CHitParameters.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCDoor.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCShop.h"
#include "engine/CTCVillage.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CTCHeroOpinionDeedLog.h"

// CHeroStatsDef: no shared header; Ego_r.pdb MoralityChangeOnVandalism (PDB +0x100; retail +0xEC: four STLport vectors 4 bytes smaller
// each; OnHit `mov eax,[edx+0ECh]`). Partial model.
class CHeroStatsDef {
    unsigned char _pad_0x00[0xec];
public:
    long MoralityChangeOnVandalism;                                     // +0xec
};

template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);       // 0x0040DCE0 <CThing*>, 0x005C05C0 <CThingCreatureBase*>

void CThingObject::OnHit(CHitParameters& params)
{
    CThingGameObject::OnHit(params);
    CThing* hitter = params.GetPThingHitting();
    if (hitter && !hitter->IsDead()) {
        if (params.IsMeleeStrike() && params.IsDirectional() && params.IsLocalised()) {
            NThingShot::CTCShotDefinitionBase* shot;
            if (GetTC(TCI_SHOT_DEFINITION, &shot))
                shot->OnHitByMelee(*hitter, params.GetPWeapon(), params.GetHitPosition(), params.GetHitDirection(), params.IsKnockDown(), params.IsBlockable());
        }
        if (GetHealth() <= 0.0f && hitter->HasTC(TCI_HERO_OPINION_DEED_LOG)) {
            bool posted = false;
            const CTCVillageMember* member;
            CTCDoor* door;
            const CTCOwnedEntity* owned;
            const CTCVillageMember* ownerMember;
            if ((PeekTC(TCI_VILLAGE_MEMBER, &member) && member->GetPVillage())
                || (GetTC(TCI_DOOR, &door) && door->PostVandalismCrimeIfDestroyed())
                || (PeekTC(TCI_OWNED_ENTITY, &owned) && owned->GetPOwner()
                    && owned->GetPOwner()->PeekTC(TCI_VILLAGE_MEMBER, &ownerMember) && ownerMember->GetPVillage())) {
                CTCHeroOpinionDeedLog* log;
                if (hitter->GetTC(&log)) {
                    CTCOwnedEntity* ownedEntity;
                    CTCBuyableHouse* house;
                    if (GetTC(TCI_OWNED_ENTITY, &ownedEntity) && IS_THING_ALIVE(ownedEntity->GetPOwner())
                        && ownedEntity->GetPOwner()->GetTC(&house) && house->GetOwnedByPlayer() && !house->GetRented()) {
                        log->PostDeed(OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION, GetPos(), this, 0, (EOpinionPostDeedType)2, false, false, 0, false, 0);
                    } else {
                        CTCShop* shop;
                        if (HasTC(TCI_DOOR) && PeekTC(TCI_OWNED_ENTITY, &owned) && owned->GetPOwner()
                            && owned->GetPOwner()->PeekTC(TCI_VILLAGE_MEMBER, &ownerMember) && ownerMember->GetPVillage()
                            && owned->GetPOwner()->GetTC(&shop) && IS_THING_ALIVE(shop->GetShopKeeper())) {
                            shop->SetAlarm(true);
                            log->PostDeed(OPINION_DEED_TYPE_CRIME_VANDALISM, GetPos(), this, (CThingAICreature*)shop->GetShopKeeper(), (EOpinionPostDeedType)0x13, true, false, 0, false, 0);
                        } else {
                            log->PostDeed(OPINION_DEED_TYPE_CRIME_VANDALISM, GetPos(), this, 0, (EOpinionPostDeedType)2, true, false, 0, false, 0);
                        }
                        CTCHeroStats* stats;
                        if (hitter->GetTC(&stats))
                            stats->SetMorality(PeekDefinitionManager().PHeroStatsDef->MoralityChangeOnVandalism + stats->Morality);
                    }
                    posted = true;
                }
            }
            if (!posted && GetDefName().GetString().Contains("BARREL")) {
                CTCHeroOpinionDeedLog* log;
                if (hitter->GetTC(&log)) {
                    CTCVillageMember* villageMember;
                    if (GetTC(TCI_VILLAGE_MEMBER, &villageMember) && villageMember->GetPVillage()) {
                        log->PostDeed(OPINION_DEED_TYPE_CRIME_VANDALISM, GetPos(), this, 0, (EOpinionPostDeedType)2, true, false, 0, false, 0);
                        CTCHeroStats* stats;
                        if (hitter->GetTC(&stats))
                            stats->SetMorality(PeekDefinitionManager().PHeroStatsDef->MoralityChangeOnVandalism + stats->Morality);
                    } else {
                        log->PostDeed(OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION, GetPos(), this, 0, (EOpinionPostDeedType)2, false, false, 0, false, 0);
                    }
                }
            }
        }
    }
    if (IsDamageable()) {
        const CTCHeroExperience* experience;
        if (HasTC(TCI_DOOR) && hitter && !hitter->IsDead() && hitter->PeekTC(TCI_HERO_EXPERIENCE, &experience)
            && !experience->IsAbleToBreakDownDoors())
            return;
        CTCOwnedEntity* ownedEntity;
        if (HasTC(TCI_DOOR) && GetTC(TCI_OWNED_ENTITY, &ownedEntity)) {
            CThing* owner = ownedEntity->GetPOwner();
            CTCVillageMember* ownerMember;
            if (owner && owner->GetTC(TCI_VILLAGE_MEMBER, &ownerMember)) {
                CTCVillage* village = ownerMember->GetPVillage();
                if (village && village->GetVillageLimbo())
                    return;
            }
        }
        if (!HasTC(TCI_WOBBLE)) {
            CCharString wobble("CTCWobble");
            AddTC(wobble, false, 0);
        }
    }
}