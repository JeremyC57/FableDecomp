// Behaviour test for CTCResurrectionItem::OnItemUsed (0x007F04C0): health restored, action finished and replaced by the
// recover-from-death action for the creature, the effect created at the creature and attached (type 8, "", 0, 0), item killed.
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
static int g_strings = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CTCDParticleEmitter;
static int g_kills = 0;
class CThing {
public:
    unsigned long  m_presentBits[9];
    CTCDParticleEmitter* PTCDriver;
    unsigned char  m_flags91;
    float          MaxHealth;
    float          Health;
    C3DVector      m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
    void Kill(bool immediately) { if (immediately) ++g_kills; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
static CThing* g_attachedTo = 0; static int g_attachOk = 0;
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing_ref, long attachType, const CCharString& helperName, long helperIndex, float offset)
    { CThing* thing = &thing_ref; g_attachedTo = thing; g_attachOk = attachType == 8 && helperName.PStringData[0] == 0 && helperIndex == 0 && offset == 0.0f; }
};
static CThing g_effect; static long g_effectType = 0; static const C3DVector* g_effectPos = 0;
CThing* CreateEffectThing(long type, const C3DVector* position, bool flag) { g_effectType = type; g_effectPos = position; return flag ? 0 : &g_effect; }
static int g_actionsAlive = 0;
class CCombatActionBase { public:
    virtual void v00() { }
    void* m_owner; int m_kind;
    ~CCombatActionBase() { --g_actionsAlive; }
};
static int g_finished = 0; static int g_setKind = 0; static void* g_setOwner = 0;
class CThingCreatureBase : public CThing {
public:
    void FinishCurrentAction() { ++g_finished; }
    bool SetCurrentAction(const CCombatActionBase& action) { g_setKind = action.m_kind; g_setOwner = action.m_owner; return true; }
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip) { ++g_actionsAlive; m_owner = owner; m_kind = flip ? 2 : 1; }
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase { public:
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(CThingCreatureBase* owner)
        : CCombatAction_GenericStrikeResponseKnockdownRecoverBase(owner, false) { }
    virtual void v00() { }
};
class CResurrectionItemDef { public: long OnUseParticleEffect; };
class CTCResurrectionItem { public:
    CThing*       ParentThing;
    const CResurrectionItemDef* PDef;
    void OnItemUsed(CThingCreatureBase& creature);
};

void CTCResurrectionItem::OnItemUsed(CThingCreatureBase& creature)
{
    creature.Health = creature.MaxHealth;
    creature.FinishCurrentAction();
    creature.SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(&creature));
    const CResurrectionItemDef* def = PDef;
    if (def && def->OnUseParticleEffect > 0) {
        CThing* effect = CreateEffectThing(def->OnUseParticleEffect, &creature.GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->PTCDriver;
            emitter->AttachToThing(creature, 8, "", 0, 0.0f);
        }
    }
    ParentThing->Kill(true);
}

int main()
{
    int bad = 0;
    CTCDParticleEmitter emitter;
    for (int i = 0; i < 9; ++i) g_effect.m_presentBits[i] = 0;
    g_effect.m_presentBits[2] = 1UL << (0x4b & 31); g_effect.PTCDriver = &emitter; g_effect.m_flags91 = 0;
    CThingCreatureBase hero; hero.MaxHealth = 200.0f; hero.Health = 0.0f; hero.m_pos.X = 1;
    CThing item; CResurrectionItemDef def; def.OnUseParticleEffect = 12;
    CTCResurrectionItem r; r.ParentThing = &item; r.PDef = &def;
    r.OnItemUsed(hero);
    if (hero.Health != 200.0f || g_finished != 1 || g_setKind != 1 || g_setOwner != &hero || g_actionsAlive != 0) bad |= 1;
    if (g_effectType != 12 || g_effectPos != &hero.m_pos || g_attachedTo != &hero || !g_attachOk || g_strings != 0) bad |= 2;
    if (g_kills != 1) bad |= 4;
    def.OnUseParticleEffect = 0; g_attachedTo = 0;
    r.OnItemUsed(hero);
    if (g_attachedTo != 0 || g_kills != 2) bad |= 8;
    r.PDef = 0;
    r.OnItemUsed(hero);
    if (g_kills != 3 || g_finished != 3) bad |= 16;
    if (bad) { std::printf("007f04c0_TEST FAIL\n"); return 1; }
    std::printf("007f04c0_TEST PASS\n");
    return 0;
}