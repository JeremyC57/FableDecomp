// CTCHealLife::StartHealingEffect() -- start the heal-life spell: the level's looping sound, the healing target (the Z-targeted live non-enemy or the caster), its heal-list / effect / accumulator entries, particle and hit-by-special-ability event, and for a self-cast every live non-enemy creature in the spell radius
// Grade: DIFFER(1980v1952) -- the loop's HealthIncreaseAccum.push_back(0.0f) is expanded inline (retail calls the folded 4-byte push_back 0x00568850 there; the first site is inline in both), so the list end stays in a register (retail spills it to [esp+44h]) and esi/edi swap; the target spill / CThingFilter_IsNot / float-zero temporaries share [esp+28h] in retail
// Next idea: a per-site inline-frequency park (chunk 794 probe: one extra `if` outlines the site). Chunk 825 tried the iterator-loop forms for(;;){if(it==end) break;}, while(it!=end), for(;;++it), it++ and &thing == target: none moves it; next, reconstruct the sibling CTCHealLife rows (FrameUpdate) to see how retail spells the same list walk
// Leaf: ?StartHealingEffect@CTCHealLife@@QAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CPlayer.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CHitParameters.h"
#include "engine/EHeroAbility.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingSearchTools.h"
#include "engine/CDefinitionManager.h"

template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CSpecialEffectsDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name,long& result) const
{
    unsigned long key=name.GetCRC();
    std::map<unsigned long,long>::const_iterator item=SpecialEffectsMap.ContainmentMap.find(key);
    if(item!=SpecialEffectsMap.ContainmentMap.end()) { result=item->second;return true; }
    return false;
}

class CSpecialAbilitiesHealLifeDef : public CDefClassBase {
    unsigned char _pad_0x25[0x3c - 0x25];
public:
    std::vector<float> HealthIncreasePerFrame;          // +0x3c
    std::vector<float> TotalHealthIncrease;             // +0x48
    std::vector<float> RadiusOfEffect;                  // +0x54
};

class CThingFilter_IsNot {
public:
    CThingFilter_IsNot(const CThing& thing) : Thing(const_cast<CThing*>(&thing)) {}
    bool operator()(const CThing& thing) const;
    CThing* Thing;
};

class CThingFilter_In2DRange {
public:
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) {}
    bool operator()(const CThing& thing) const;
    C3DVector Pos;
    float Range;
};

class CTCEnemy : public CTCBase {
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};

class CTCSpecialAbilities {
public:
    static CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& pos);   // 0x0072AB70
};

class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    void AttachToThing(CThing& thing, long priority, const CCharString& dummy = "", long flags = 0, float delay = 0.0f);   // 0x006E0BE0
};

namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
};
class CEventHitBySpecialAbility : public CEventBase {
public:
    CEventHitBySpecialAbility(const CHitParameters& params, EHeroAbility ability);   // 0x00811830
private:
    unsigned char _pad[0x14];
};
}
class CTCEntityEvents : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENTITY_EVENTS; }
    void AddEvent(NEntityEvents::CEventBase* event);                                 // 0x008113C0
};

template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing.Get() && !thing->IsDead(); }
template<class T> inline bool __fastcall IS_THING_ALIVE(T* const& thing) { return thing && !thing->IsDead(); }

class CTCHealLife : public CTCBase {
public:
    void StartHealingEffect();
private:
    CCharString GetLevelEffectName();                                           // 0x007C3EB0
    CDefPointer<const CSpecialAbilitiesHealLifeDef> PDef;                       // +0x0c
    float TotalHealthAccum;                                                     // +0x10
    unsigned long LoopingSound;                                                 // +0x14
    long SpellLevel;                                                            // +0x18
    CIntelligentPointer<CThing> PItemRemoved;                                   // +0x1c
    std::list<CIntelligentPointer<CThingCreatureBase> > ThingsToHeal;           // +0x24
    std::vector<CIntelligentPointer<CThing> > HealingEffects;                   // +0x28
    std::vector<float> HealthIncreaseAccum;                                     // +0x34
    CIntelligentPointer<CThing> PHealingTarget;                                 // +0x40
};

void CTCHealLife::StartHealingEffect()
{
    CThing& thing = GetParentThing();
    CTCSoundPlayer* soundPlayer;
    if (thing.GetTC(&soundPlayer)) {
        switch (SpellLevel) {
        case 0: LoopingSound = soundPlayer->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_1", false, false, false, -1); break;
        case 1: LoopingSound = soundPlayer->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_2", false, false, false, -1); break;
        case 2: LoopingSound = soundPlayer->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_3", false, false, false, -1); break;
        case 3: LoopingSound = soundPlayer->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_4", false, false, false, -1); break;
        }
    }
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> effects;
    CCharString name = GetLevelEffectName();
    float radius = PDef->RadiusOfEffect[SpellLevel];
    PHealingTarget = &thing;
    const CTCTargetingBase* targeting;
    if (thing.PeekTC(&targeting)) {
        CThing* targeted = targeting->GetPTargetedThing();
        if (IS_THING_ALIVE(targeted) && thing.GetPlayer().IsZTargeting())
            PHealingTarget = targeted;
    }
    if (CTCEnemy::AreThingsEnemies(*PHealingTarget.Get(), thing))
        PHealingTarget = &thing;
    if (PHealingTarget->GetTypeFlags() & 8) {
        CThing* target = PHealingTarget.Get();
        if (IS_THING_ALIVE(target)) {
            ThingsToHeal.push_back(static_cast<CThingCreatureBase*>(target));
            HealingEffects.push_back(CIntelligentPointer<CThing>(0));
            HealthIncreaseAccum.push_back(0.0f);
            if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(name, effect)) {
                CIntelligentPointer<CThing> particle(CTCSpecialAbilities::CreateParticleEffect(effect, target->GetPos()));
                if (IS_THING_ALIVE(particle)) {
                    CTCDParticleEmitter* emitter;
                    particle->GetTC(&emitter);
                    emitter->AttachToThing(*target, 0x28);
                    HealingEffects.back() = particle;
                }
            }
            CTCEntityEvents* events;
            if (target->GetTC(&events)) {
                CHitParameters hit;
                hit.SetThingHitting(thing);
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_HEAL_LIFE_SPELL));
            }
            if (target == &thing) {
                C3DVector pos = target->GetPos();
                C2DBoxI box(C2DCoordI((long)pos.X, (long)pos.Y), (long)radius);
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(box,
                    GFFilter_Bind2(CThingFilter_IsNot(*target), CThingFilter_In2DRange(pos, radius)), ThingsToHeal);
                for (std::list<CIntelligentPointer<CThingCreatureBase> >::iterator it = ThingsToHeal.begin(), end = ThingsToHeal.end(); it != end; ++it) {
                    CIntelligentPointer<CThingCreatureBase> creature = *it;
                    if (creature.Get() == target)
                        continue;
                    HealingEffects.push_back(CIntelligentPointer<CThing>(0));
                    HealthIncreaseAccum.push_back(0.0f);
                    if (IS_THING_ALIVE(creature) && !CTCEnemy::AreThingsEnemies(*target, *creature.Get())) {
                        if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(name, effect)) {
                            CIntelligentPointer<CThing> particle(CTCSpecialAbilities::CreateParticleEffect(effect, creature->GetPos()));
                            if (IS_THING_ALIVE(particle)) {
                                CTCDParticleEmitter* emitter = 0;
                                particle->GetTC(&emitter);
                                emitter->AttachToThing(*creature.Get(), 0x28);
                                HealingEffects.back() = particle;
                            }
                        }
                        if (creature->GetTC(&events)) {
                            CHitParameters hit;
                            hit.SetThingHitting(thing);
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_HEAL_LIFE_SPELL));
                        }
                    }
                }
            }
        }
    }
}
