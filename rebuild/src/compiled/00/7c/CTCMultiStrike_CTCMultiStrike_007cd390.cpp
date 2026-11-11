// CTCMultiStrike::CTCMultiStrike(CThing& thing) -- set up the multi-strike spell component: members cleared, its special-ability def fetched, the strike effect put on the active melee weapon (or the thing in the primary carry slot), the spell level read from the hero abilities (1 without them), the level's CAST_EFFECT particle effect created and attached to the caster, the level's cast sound played and a SIMPLE_NOISE made
// Leaf: ??0CTCMultiStrike@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CD390, 796 bytes of code (`ret 4` at 0x007CD698, returns this) then its own 4-entry case table
//   0x007CD69C..0x007CD6AB; the catalog's 832 also spans two unreferenced stubs (SPLITS.tsv). No EH frame.
// Identity: the vtable stored after CTCBase::CTCBase is 0x0126EE0C (CTCMultiStrike, as the landed FinalCleanup / dtor rows); the def
//   name literal "SPECIAL_ABILITIES_MULTI_STRIKE_DEF" 0x01235A9C; Ego_r.pdb / FableWin.pdb ??0CTCMultiStrike@@QAE@AAVCThing@@@Z (FableWin
//   0x0272FCE0, after the Alloc 0x01BB4DF8 that news the component).
// Callees: CTCBase::CTCBase 0x00686800; CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 (CIntelligentPointer
//   <CThing> vtable 0x01238C6C); CCharString(const char*, long) 0x0099EBF0 / CCharString() 0x0099EAE0 / operator= 0x0099EFE0 / ~ 0x0099EAE0;
//   CGameDefinitionManager::Get 0x0044C6B0 (twice), CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, GetDef
//   <CSpecialAbilitiesMultiStrikeDef> 0x007CD760; CVectorMap::LowerBound const 0x004365B0 (PeekTC 0x13 / 0x46 / 0x6F) and 0x0040F020
//   (GetTC 0x4B / 0x10); CTCCarrying::GetThingInPrimarySlot 0x006A1440; CTCMultiStrike::ApplyEffectOnWeapon 0x007CCF50;
//   CTCInventoryAbilities::GetAbilityLevel 0x005EDE93; CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580; CSpecialEffectsDef::
//   GetEffectFromString 0x00669D30; CThing::GetPos 0x004C73D0; CTCSpecialAbilities::CreateParticleEffect 0x0072AB70 (__fastcall);
//   CTCDParticleEmitter::SetAlwaysUpdate 0x006E0590 / AttachToThing 0x006E0BE0; CDefStringTable::GetString 0x009D49B0 on GDefStringTable
//   0x013CA828; CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00; CThingCreatureBase::MakeNoise 0x00662110.
// Data: "CAST_EFFECT_LEVEL1".."4" 0x0126EF68 / 0x0126EFA4 / 0x0126EF90 / 0x0126EF7C, "" 0x0122D70E, "SIMPLE_NOISE" 0x01268A5C.
// Shape: GetEffectFromString's same-TU noinline body (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, as 0x007941D0) lets VC7.1 treat the
//   effect index as uncaptured, so it shares [esp+14h] with the five interface keys and the CCharString temporaries (frame 8; with only
//   the declaration the keys take a slot of their own). The switch has an explicit case 0 beside an identical default, tail-merged into
//   one block (the 4-entry table from 0). The weapon / carrying / abilities lookups are the const PeekTC (LowerBound 0x004365B0); the
//   emitter and sound-player lookups the non-const GetTC (0x0040F020). The parent is held in a reference for the sound and the noise.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"
#include "engine/CTCMultiStrike.h"

CTCMultiStrike::CTCMultiStrike(CThing& thing)
    : CTCBase(thing)
    , SpellLevel(0)
    , StrikeStarted(false)
    , DelayTimer(0)
    , NumHits(0)
    , PHitLocation(0)
    , PWeapon(0)
    , PTarget(0)
{
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_MULTI_STRIKE_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index, &PDef);
    CThing* weapon = 0;
    const CTCInventoryWeapons* inventory_weapons;
    const CTCCarrying* carrying;
    if (GetParentThing().PeekTC(&inventory_weapons))
        weapon = inventory_weapons->GetActiveMeleeWeapon();
    else if (GetParentThing().PeekTC(&carrying))
        weapon = carrying->GetThingInPrimarySlot();
    if (weapon)
        ApplyEffectOnWeapon(weapon);
    const CTCInventoryAbilities* inventory_abilities;
    if (GetParentThing().PeekTC(&inventory_abilities))
        SpellLevel = inventory_abilities->GetAbilityLevel(HERO_ABILITY_DOUBLE_STRIKE);
    else
        SpellLevel = 1;
    CDefPointer<const CSpecialEffectsDef> special_effects;
    long effect = 0;
    CCharString effect_name;
    switch (SpellLevel) {
    case 0:
        effect_name = "CAST_EFFECT_LEVEL1";
        break;
    case 1:
        effect_name = "CAST_EFFECT_LEVEL2";
        break;
    case 2:
        effect_name = "CAST_EFFECT_LEVEL3";
        break;
    case 3:
        effect_name = "CAST_EFFECT_LEVEL4";
        break;
    default:
        effect_name = "CAST_EFFECT_LEVEL1";
        break;
    }
    if (PDef->GetSubDef(&special_effects) && special_effects->GetEffectFromString(effect_name, effect)) {
        CThing* effect_thing = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
        CTCDParticleEmitter* emitter = 0;
        effect_thing->GetTC(&emitter);
        emitter->SetAlwaysUpdate(true);
        emitter->AttachToThing(GetParentThing(), 0x28, "", 0, 0);
    }
    CThing& parent = GetParentThing();
    CTCSoundPlayer* sound_player;
    if (parent.GetTC(&sound_player))
        sound_player->PlayAttachedCriteria(PDef->Sounds[SpellLevel], true, false, false, -1);
    static_cast<CThingCreatureBase&>(parent).MakeNoise("SIMPLE_NOISE");
}