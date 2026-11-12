// Behaviour test for 0x008C2AD0 CCombatAction_WillHealInto::FinishAction: the function's logic over a small stand-alone model
// (def with or without a HEAL effect, live / dead / emitter-less effect things, creature with or without a heal-life component).
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* text) { std::strcat(g_log, text); std::strcat(g_log, ";"); }

class CSpecialEffectsDef { public:
    long RefCount;
    bool HasHeal;
    long HealEffect;
    bool GetEffectFromString(const char* name, long& effect) const
    {
        if (HasHeal && std::strcmp(name, "HEAL") == 0) { effect = HealEffect; return true; }
        return false;
    }
};
class CThing;
class CTCDParticleEmitter { public:
    CThing* Attached; long AttachType; long HelperIndex; float Offset; bool EmptyHelper;
    void AttachToThing(CThing& thing, long attach_type, const char* helper, long helper_index, float offset)
    {
        Attached = &thing; AttachType = attach_type; EmptyHelper = helper[0] == 0; HelperIndex = helper_index; Offset = offset;
        Log("attach");
    }
};
class CTCHealLife { public: long SpellLevel; void SetAICast(long level) { SpellLevel = level; Log("cast"); } };
class CThing { public:
    unsigned long UsingComponents[9];
    CTCDParticleEmitter* PTCDriver;
    CSpecialEffectsDef* PSpecialEffects;      // what the thing's def hands out as its special-effects sub def
    bool Dead;
    CTCHealLife HealLife;
    long CurrentAction;
    bool HasTC(long type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    bool GetSubDef(CSpecialEffectsDef** out) const
    {
        if (!PSpecialEffects) return false;
        *out = PSpecialEffects; ++PSpecialEffects->RefCount; return true;
    }
    CTCHealLife* AddTC(const char* name)
    {
        if (std::strcmp(name, "CTCHealLife") != 0) return 0;
        UsingComponents[153 >> 5] |= 1UL << (153 & 31); Log("addtc"); return &HealLife;
    }
    void SetCurrentAction(long action) { CurrentAction = action; Log("set"); }
};
static CThing* g_effect_thing; static long g_created_effect; static int g_creates;
static CThing* CreateEmitter(long effect) { g_created_effect = effect; ++g_creates; Log("create"); return g_effect_thing; }

class CCombatAction_WillHealInto { public:
    CThing* PCreature;
    long SpellLevel;
    void BaseFinishAction() { Log("base"); }
    void FinishAction()
    {
        CSpecialEffectsDef* pspecial_effects = 0;
        CThing* pfx = 0;
        {
            long effect = 0;
            PCreature->GetSubDef(&pspecial_effects);
            if (pspecial_effects) {
                if (pspecial_effects->GetEffectFromString("HEAL", effect))
                    pfx = CreateEmitter(effect);
            }
        }
        if (pfx && !pfx->Dead) {
            if (pfx->HasTC(75))
                pfx->PTCDriver->AttachToThing(*PCreature, 10, "", 0, 0.0f);
        }
        CThing* pcreature = PCreature;
        BaseFinishAction();
        if (!pcreature->HasTC(153)) {
            CTCHealLife* ptc_heal_life = pcreature->AddTC("CTCHealLife");
            ptc_heal_life->SetAICast(SpellLevel);
        }
        pcreature->SetCurrentAction(0x8C3930);
        if (pspecial_effects) --pspecial_effects->RefCount;      // the def pointer's release, last
    }
};

static bool Run(bool has_def, bool has_heal, bool have_thing, bool dead, bool has_emitter, bool has_heal_tc, const char* expect)
{
    CSpecialEffectsDef def; def.RefCount = 1; def.HasHeal = has_heal; def.HealEffect = 77;
    CTCDParticleEmitter emitter; std::memset(&emitter, 0, sizeof(emitter));
    CThing fx; std::memset(&fx, 0, sizeof(fx));
    fx.Dead = dead; fx.PTCDriver = &emitter;
    if (has_emitter) fx.UsingComponents[75 >> 5] |= 1UL << (75 & 31);
    CThing creature; std::memset(&creature, 0, sizeof(creature));
    creature.PSpecialEffects = has_def ? &def : 0;
    if (has_heal_tc) creature.UsingComponents[153 >> 5] |= 1UL << (153 & 31);
    creature.HealLife.SpellLevel = -1;
    g_effect_thing = have_thing ? &fx : 0; g_created_effect = -1; g_creates = 0; g_log[0] = 0;
    CCombatAction_WillHealInto action; action.PCreature = &creature; action.SpellLevel = 3;
    action.FinishAction();
    if (std::strcmp(g_log, expect) != 0) { std::printf("log '%s' expected '%s'\n", g_log, expect); return false; }
    if (def.RefCount != 1) return false;                                   // the sub def reference is given back
    if (creature.CurrentAction != 0x8C3930) return false;                  // always moves on to the release action
    if (!creature.HasTC(153)) return false;                                // the creature ends with a heal-life component
    if (creature.HealLife.SpellLevel != (has_heal_tc ? -1 : 3)) return false;   // cast level set only on a new component
    if (has_def && has_heal) { if (g_creates != 1 || g_created_effect != 77) return false; }
    else if (g_creates != 0) return false;
    bool attached = has_def && has_heal && have_thing && !dead && has_emitter;
    if (attached) {
        if (emitter.Attached != &creature || emitter.AttachType != 10 || !emitter.EmptyHelper || emitter.HelperIndex != 0
            || emitter.Offset != 0.0f) return false;
    } else if (emitter.Attached != 0) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(true,  true,  true,  false, true,  false, "create;attach;base;addtc;cast;set;");
    ok = ok && Run(true,  true,  true,  false, true,  true,  "create;attach;base;set;");
    ok = ok && Run(true,  true,  true,  true,  true,  false, "create;base;addtc;cast;set;");      // dead effect thing: no attach
    ok = ok && Run(true,  true,  true,  false, false, false, "create;base;addtc;cast;set;");      // no particle emitter driver
    ok = ok && Run(true,  true,  false, false, true,  false, "create;base;addtc;cast;set;");      // Create returned nothing
    ok = ok && Run(true,  false, true,  false, true,  false, "base;addtc;cast;set;");             // def maps no HEAL effect
    ok = ok && Run(false, true,  true,  false, true,  true,  "base;set;");                        // no special-effects sub def
    if (!ok) { std::printf("008c2ad0_TEST FAIL\n"); return 1; }
    std::printf("008c2ad0_TEST PASS\n");
    return 0;
}