// NThingShot::CTCShotDefinitionFireballSpell::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*) -- run the base
//   shot collision, set fire to a creature-type thing (type-flag bit 8: reset its CTCBurntEffect, else add one), tell a live thing's entity events it was
//   hit by the fireball (CEventHitBySpecialAbility, HERO_ABILITY_FIREBALL_SPELL = 16), then play the explosion sound for the spell's power-up level
//   (the CFireballSpellLevelDef sub-def's SpellLevel, 0 without one) through the parent's sound player.
// Leaf: ?OnCollision@CTCShotDefinitionFireballSpell@NThingShot@@MAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z
// Flags: none (TU pragmas optimize("y", off) + optimize("s", on))
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BDE31, 555 bytes, thiscall `leave ; ret 10h`; vtable 0x01241064 slot 28 (Ego_r.pdb MAE).
// Callees: CTCShotDefinitionBase::OnCollision 0x007BF510, std::bitset<274>::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020,
//   CTCBurntEffect::Reset 0x007CDD10, CThing::AddTC 0x004C9D60, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IS_THING_ALIVE<CThing*>
//   0x0040DCE0, CHitParameters 0x007A3BC0 / ~ 0x00789358, CHitParameters::SetThingHitting 0x008685F0, CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, operator new 0x00BFEA1A, CEventHitBySpecialAbility 0x00811830, CTCEntityEvents::AddEvent 0x008113C0,
//   CDefClassBase::GetSubDef<CFireballSpellLevelDef> 0x007BE88C, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00.
// Shape: GetSubDef<CFireballSpellLevelDef> is defined later in this retail TU, so its real body is visible but out of line
//   (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, GOTCHAS chunk Q19): VC7.1 then knows it only writes through the out pointer, keeps
//   the level in ebx, shares the dead `thing` slot with the def and releases it in place (`dec [ecx+4] ; jne`). IS_THING_ALIVE is the
//   out-of-line COMDAT with its body visible (FABLE_RETAIL_IS_THING_ALIVE_NOINLINE). Each switch arm passes its criteria name as an
//   implicit temporary (its own slot, one shared ~CCharString tail), as the landed CTCForcePushPower::PlayCastSound 0x007A5580.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CHitParameters.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/CFireballSpellLevelDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/ThingAlive.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

// CTCBurntEffect has no shared header: the two members this row needs (FableWin.pdb ?StatGetInterfaceType@CTCBurntEffect@@SI?AW4ETCInterfaceType@@XZ,
// TCI_BURNT_EFFECT 137; Ego_r ?Reset@CTCBurntEffect@@QAEXXZ).
class CTCBurntEffect : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BURNT_EFFECT; }
    void Reset();                                                       // 0x007CDD10
};

namespace NThingShot {
class CTCShotDefinitionFireballSpell : public CTCShotDefinitionBase {
protected:
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);
};

void CTCShotDefinitionFireballSpell::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CTCShotDefinitionBase::OnCollision(type, position, normal, thing);
    // `test byte ptr [esi+6Ch],8` (0x007BDE55): bit 8 is in CThingCreatureBase::StatGetTypeFlags (0xF) and CThingAICreature's (0x1F),
    // not in CThingGameObject's (7) or CThingBuilding's (0x47) -- the creature type bit; no named THING_TYPE constant in the headers.
    if (thing && (thing->GetTypeFlags() & 8)) {
        CTCBurntEffect* burnt;
        if (thing->GetTC(&burnt))
            burnt->Reset();
        else
            thing->AddTC("CTCBurntEffect", true, 0);
    }
    CTCEntityEvents* events;
    if (IS_THING_ALIVE(thing) && thing->GetTC(&events)) {
        CHitParameters hit;
        hit.SetThingHitting(*PShooter.Get());
        events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_FIREBALL_SPELL));
    }
    CDefPointer<const CFireballSpellLevelDef> def;
    long level = 0;
    if (PShotDef->GetSubDef(&def))
        level = def->SpellLevel;
    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(&sound)) {
        switch (level) {
        case 0: sound->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_EXPLOSION_NO_POWERUP", true, false, false, -1); break;
        case 1: sound->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_1", true, false, false, -1); break;
        case 2: sound->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_2", true, false, false, -1); break;
        case 3: sound->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_3", true, false, false, -1); break;
        }
    }
}
}