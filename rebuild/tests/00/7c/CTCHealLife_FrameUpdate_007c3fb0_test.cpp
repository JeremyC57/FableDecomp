#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/GFDebugPDynamicCast.h"
#include "stlport/fable_containers.h"
#include <math.h>
#include <cstdio>
#include <cstring>

static unsigned failures;
#define CHECK(condition) do { if (!(condition)) { \
    if (failures < 20) std::printf("line %d: %s\n", __LINE__, #condition); ++failures; \
} } while (0)

class FixtureString {
public:
    FixtureString(const char* value) : text(value) {}
    const char* text;
};

struct FixtureEmitter;
struct FixtureThing {
    C3DVector position;
    FixtureEmitter* emitter;
    bool dead, enemy;
    unsigned emitterLookups;

    FixtureThing() : emitter(0), dead(false), enemy(false), emitterLookups(0) {}
    const C3DVector& GetPos() const { return position; }
    bool GetTC(FixtureEmitter** output)
    {
        ++emitterLookups;
        CHECK(emitter != 0);
        *output = emitter;
        return emitter != 0;
    }
};

struct FixtureEmitter {
    unsigned attached, ended;
    const FixtureThing* owner;

    FixtureEmitter() : attached(0), ended(0), owner(0) {}
    void AttachToThing(FixtureThing& thing, long kind, const FixtureString& helper, long index, float offset)
    {
        CHECK(kind == 0x28 && !std::strcmp(helper.text, "") && index == 0 && offset == 0.0f);
        owner = &thing;
        ++attached;
    }
    void EnterTimelineEnd() { ++ended; }
};

struct FixtureCreature : FixtureThing {
    unsigned noise, healingCalls, removal;
    float healed;

    FixtureCreature() : noise(0), healingCalls(0), removal(0), healed(0.0f) {}
    void MakeNoise(const FixtureString& name) { CHECK(!std::strcmp(name.text, "SIMPLE_NOISE")); ++noise; }
    void ModifyHealth(float value, bool show) { CHECK(show && value > 0.0f); healed += value; ++healingCalls; }
    void RemoveTC(ETCInterfaceType type) { CHECK(type == TCI_HEAL_LIFE); ++removal; }
};

template<class T> class FixtureIntelligentPointer {
public:
    FixtureIntelligentPointer(T* value = 0) : p(value) {}
    T* operator->() const { return p; }
    T& operator*() const { return *p; }
    bool operator==(int null) const { CHECK(null == 0); return p == 0; }
    T* p;
};
template<class T> bool IS_THING_ALIVE(const FixtureIntelligentPointer<T>& pointer)
{
    return pointer.p && !pointer->dead;
}

struct FixtureEnemy {
    static unsigned queries;
    static const FixtureThing* enemyBase;
    static bool AreThingsEnemies(const FixtureThing& base, const FixtureThing& target)
    {
        CHECK(&base == enemyBase && !base.dead && !target.dead);
        ++queries;
        return target.enemy;
    }
};
unsigned FixtureEnemy::queries;
const FixtureThing* FixtureEnemy::enemyBase;

struct FixtureSpecialAbilities {
    static FixtureThing* particles[3];
    static const C3DVector* positions[3];
    static unsigned creates;
    static FixtureThing* CreateParticleEffect(long effect, const C3DVector& position)
    {
        CHECK(effect == 91);
        CHECK(creates < 3 && &position == positions[creates]);
        if (creates < 3)
            return particles[creates++];
        return 0;
    }
};
FixtureThing* FixtureSpecialAbilities::particles[3];
const C3DVector* FixtureSpecialAbilities::positions[3];
unsigned FixtureSpecialAbilities::creates;

struct FixtureEffects {
    static unsigned searches;
    bool found;
    bool GetEffectFromString(const FixtureString& effectname, long& effect) const
    {
        CHECK(!std::strcmp(effectname.text, "HEAL_EFFECT"));
        ++searches;
        if (found)
            effect = 91;
        return found;
    }
};
unsigned FixtureEffects::searches;

template<class T> class FixtureDefPointer {
public:
    FixtureDefPointer(T* value = 0) : p(value) {}
    T* operator->() const { return p; }
    T* p;
};

struct FixtureDef {
    float HealthIncreasePerFrame[2], TotalHealthIncrease[2];
    const FixtureEffects* effects;
    static unsigned lookups;

    bool GetSubDef(FixtureDefPointer<const FixtureEffects>* output) const
    {
        ++lookups;
        if (effects)
            output->p = effects;
        return effects != 0;
    }
};
unsigned FixtureDef::lookups;

struct FixtureHealLife {
    FixtureCreature* owner;
    FixtureDefPointer<const FixtureDef> PDef;
    FixtureIntelligentPointer<FixtureThing> PHealingTarget;
    std::list<FixtureIntelligentPointer<FixtureCreature> > ThingsToHeal;
    std::vector<FixtureIntelligentPointer<FixtureThing> > HealingEffects;
    std::vector<float> HealthIncreaseAccum;
    long SpellLevel;
    float TotalHealthAccum;
    unsigned nameReads;

    FixtureString GetLevelEffectName() { ++nameReads; return FixtureString("HEAL_EFFECT"); }
    FixtureThing& GetParentThing() { return *owner; }
    void FrameUpdate();
};

#define CTCHealLife FixtureHealLife
#define CThingCreatureBase FixtureCreature
#define CThing FixtureThing
#define CCharString FixtureString
#define CDefPointer FixtureDefPointer
#define CSpecialEffectsDef FixtureEffects
#define CIntelligentPointer FixtureIntelligentPointer
#define CTCEnemy FixtureEnemy
#define CTCSpecialAbilities FixtureSpecialAbilities
#define CTCDParticleEmitter FixtureEmitter

void CTCHealLife::FrameUpdate()
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    pcreature->MakeNoise("SIMPLE_NOISE");
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    CCharString effectname = GetLevelEffectName();
    long index = 0;
    if (false) {} else
    for (std::list<CIntelligentPointer<CThingCreatureBase> >::iterator it = ThingsToHeal.begin(), endit = ThingsToHeal.end();
            it != endit; ++it) {
        CIntelligentPointer<CThingCreatureBase> something = *it;
        if (IS_THING_ALIVE(something) && IS_THING_ALIVE(PHealingTarget)) {
            if (!CTCEnemy::AreThingsEnemies(*PHealingTarget, *something)) {
                if (HealingEffects[index] == 0 && PDef->GetSubDef(&pspecial_effects) &&
                        pspecial_effects->GetEffectFromString(effectname, effect)) {
                    CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, something->GetPos()));
                    if (IS_THING_ALIVE(fx)) {
                        CTCDParticleEmitter* ptc_emitter;
                        fx->GetTC(&ptc_emitter);
                        ptc_emitter->AttachToThing(*something, 0x28, "", 0, 0.0f);
                        HealingEffects[index] = fx;
                    }
                }
                HealthIncreaseAccum[index] += PDef->HealthIncreasePerFrame[SpellLevel];
                if (HealthIncreaseAccum[index] > 1.0f) {
                    float man = (float)floor(HealthIncreaseAccum[index]);
                    something->ModifyHealth(man, true);
                    HealthIncreaseAccum[index] -= man;
                }
            }
        }
        else if (IS_THING_ALIVE(HealingEffects[index])) {
            CTCDParticleEmitter* ptcd_particle_emitter = 0;
            HealingEffects[index]->GetTC(&ptcd_particle_emitter);
            ptcd_particle_emitter->EnterTimelineEnd();
        }
        ++index;
    }
    TotalHealthAccum += PDef->HealthIncreasePerFrame[SpellLevel];
    if (TotalHealthAccum >= PDef->TotalHealthIncrease[SpellLevel] || !IS_THING_ALIVE(PHealingTarget))
        pcreature->RemoveTC(TCI_HEAL_LIFE);
}

#undef CTCHealLife
#undef CThingCreatureBase
#undef CThing
#undef CCharString
#undef CDefPointer
#undef CSpecialEffectsDef
#undef CIntelligentPointer
#undef CTCEnemy
#undef CTCSpecialAbilities
#undef CTCDParticleEmitter

int main()
{
    const float initial[] = {0.0f, 0.75f, 1.0f, 2.75f};
    for (unsigned scenario = 0; scenario < 2592; ++scenario) {
        unsigned digits = scenario;
        const int focus = digits % 3; digits /= 3;
        const bool hasDefinition = digits % 2 != 0; digits /= 2;
        const bool found = digits % 2 != 0; digits /= 2;
        const long level = digits % 2; digits /= 2;
        const bool expires = digits % 2 != 0; digits /= 2;
        const unsigned count = digits % 4; digits /= 4;
        const int offset = digits % 27;

        FixtureHealLife spell;
        FixtureCreature caster, targets[3];
        FixtureThing existing[3], created[3], primary;
        FixtureEmitter previousEmitters[3], newEmitters[3];
        FixtureDef definition;
        FixtureEffects effects;
        primary.dead = focus == 2;
        FixtureEnemy::enemyBase = &primary;
        effects.found = found;
        definition.effects = hasDefinition ? &effects : 0;
        definition.HealthIncreasePerFrame[0] = 0.25f;
        definition.HealthIncreasePerFrame[1] = 1.25f;
        definition.TotalHealthIncrease[level] = expires ? 2.0f : 100.0f;
        definition.TotalHealthIncrease[1-level] = 0.0f;
        spell.owner = &caster;
        spell.PDef.p = &definition;
        spell.PHealingTarget.p = focus ? &primary : 0;
        spell.SpellLevel = level;
        spell.TotalHealthAccum = 2.0f - definition.HealthIncreasePerFrame[level];
        spell.nameReads = 0;

        FixtureThing* expectedEffects[3] = {0, 0, 0};
        bool expectedEnd[3] = {false, false, false};
        bool expectedAttach[3] = {false, false, false};
        float expectedAccum[3] = {0.0f, 0.0f, 0.0f};
        float expectedHealth[3] = {0.0f, 0.0f, 0.0f};
        unsigned expectedCreates = 0, expectedLookups = 0, expectedSearches = 0, expectedEnemies = 0;
        for (unsigned i = 0; i != count; ++i) {
            const int targetState = (offset + i) % 3;
            const int priorState = (offset / 3 + i) % 3;
            const int creationState = (offset / 9 + i) % 3;
            targets[i].dead = targetState == 2;
            targets[i].enemy = ((scenario / 7 + i) & 1) != 0;
            targets[i].position = C3DVector((float)i, (float)(i+2), (float)(i+4));
            existing[i].dead = priorState == 2;
            existing[i].emitter = &previousEmitters[i];
            created[i].dead = creationState == 2;
            created[i].emitter = &newEmitters[i];
            spell.ThingsToHeal.push_back(FixtureIntelligentPointer<FixtureCreature>(targetState ? &targets[i] : 0));
            spell.HealingEffects.push_back(FixtureIntelligentPointer<FixtureThing>(priorState ? &existing[i] : 0));
            spell.HealthIncreaseAccum.push_back(initial[(offset + i) % 4]);
            expectedEffects[i] = spell.HealingEffects[i].p;
            expectedAccum[i] = spell.HealthIncreaseAccum[i];

            if (targetState == 1 && focus == 1) {
                ++expectedEnemies;
                if (!targets[i].enemy) {
                    if (!priorState) {
                        ++expectedLookups;
                        if (hasDefinition) {
                            ++expectedSearches;
                            if (found) {
                                FixtureSpecialAbilities::positions[expectedCreates] = &targets[i].position;
                                FixtureSpecialAbilities::particles[expectedCreates++] = creationState ? &created[i] : 0;
                                if (creationState == 1) {
                                    expectedAttach[i] = true;
                                    expectedEffects[i] = &created[i];
                                }
                            }
                        }
                    }
                    expectedAccum[i] += definition.HealthIncreasePerFrame[level];
                    if (expectedAccum[i] > 1.0f) {
                        expectedHealth[i] = (float)(int)expectedAccum[i];
                        expectedAccum[i] -= expectedHealth[i];
                    }
                }
            } else if (priorState == 1) {
                expectedEnd[i] = true;
            }
        }

        FixtureDef::lookups = FixtureEffects::searches = FixtureEnemy::queries = FixtureSpecialAbilities::creates = 0;
        spell.FrameUpdate();
        CHECK(caster.noise == 1 && spell.nameReads == 1 && spell.TotalHealthAccum == 2.0f);
        CHECK(caster.removal == (expires || focus != 1 ? 1U : 0U));
        CHECK(FixtureEnemy::queries == expectedEnemies && FixtureDef::lookups == expectedLookups &&
              FixtureEffects::searches == expectedSearches && FixtureSpecialAbilities::creates == expectedCreates);

        for (unsigned i = 0; i != count; ++i) {
            CHECK(spell.HealthIncreaseAccum[i] == expectedAccum[i] && targets[i].healed == expectedHealth[i]);
            CHECK(targets[i].healingCalls == (expectedHealth[i] > 0.0f ? 1U : 0U));
            CHECK(spell.HealingEffects[i].p == expectedEffects[i]);
            CHECK(previousEmitters[i].ended == (expectedEnd[i] ? 1U : 0U));
            CHECK(newEmitters[i].attached == (expectedAttach[i] ? 1U : 0U));
            CHECK(newEmitters[i].owner == (expectedAttach[i] ? &targets[i] : 0));
        }
    }
    std::puts(failures ? "007c3fb0_TEST FAIL" : "007c3fb0_TEST PASS");
    return failures ? 1 : 0;
}