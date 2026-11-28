// Behaviour test for FixtureCCreatureAction_BriarRoseConcentrateInto::Initialise (0x008C1C30): unsheathe, POWER_UP_INTO, base
// Initialise; a mode manager plus a live enemy adds FixtureCModeBriarRoseConcentrate(creature, enemy, Imposter); the def's
// TELEPORT_INTO effect is created at the creature when the def has effects and knows the name.
#include <cstdio>
#include <cstring>

class FixtureC3DVector { public: float X, Y, Z; };
class FixtureCThing;
static int g_unsheathes = 0, g_baseInit = 0, g_modes = 0, g_effects = 0; static char g_anim[32];
static FixtureCThing* g_modeCreature = 0; static FixtureCThing* g_modeEnemy = 0; static bool g_modeImposter = false;
static long g_effectType = -1; static const FixtureC3DVector* g_effectPos = 0;
class FixtureCCharString { public: const char* s; FixtureCCharString(const char* t) : s(t) {} };
class FixtureCSpecialEffectsDef { public:
    long m_refCount; int Destroyed; const char* Known; long KnownType;
    void Destroy() { ++Destroyed; }
    bool GetEffectFromString(const FixtureCCharString& n, long& type) const { if (std::strcmp(n.s, Known) != 0) return false; type = KnownType; return true; }
};
template <class T> class FixtureCDefPointer { public:
    T* m_p;
    FixtureCDefPointer() : m_p(0) { }
    ~FixtureCDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
};
class FixtureCDefClassBase { public:
    long m_refCount; int Destroyed; FixtureCSpecialEffectsDef* Effects;
    void Destroy() { ++Destroyed; }
    bool GetSubDef(FixtureCDefPointer<FixtureCSpecialEffectsDef>& out) { if (!Effects) return false; ++Effects->m_refCount; out.m_p = Effects; return true; }
};
struct FixtureCDefClassReference {
    FixtureCDefClassBase* m_p;
    FixtureCDefClassReference(FixtureCDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~FixtureCDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    FixtureCDefClassBase* operator->() const { return m_p; }
};
class FixtureCTCCreatureModeManager;
class FixtureCThing { public:
    unsigned char m_flags91; FixtureCDefClassBase* m_pDef; FixtureC3DVector Pos; FixtureCTCCreatureModeManager* Modes;
    const FixtureC3DVector& GetPosition() const { return Pos; }
    void UnsheathePrimaryWeapon() { ++g_unsheathes; }
    bool GetInterface(FixtureCTCCreatureModeManager** out, long) { if (!Modes) return false; *out = Modes; return true; }
};
FixtureCThing* CreateEffectThing(long type, const FixtureC3DVector* pos, bool) { g_effectType = type; g_effectPos = pos; ++g_effects; return 0; }
struct FixtureCModeBriarRoseConcentrate {
    FixtureCModeBriarRoseConcentrate(FixtureCThing* c, FixtureCThing* e, bool i) { g_modeCreature = c; g_modeEnemy = e; g_modeImposter = i; }
};
struct FixtureCCountedModePointer { FixtureCModeBriarRoseConcentrate* m_pObject; FixtureCCountedModePointer(FixtureCModeBriarRoseConcentrate* o) : m_pObject(o) {} };
class FixtureCTCCreatureModeManager { public: bool AddMode(FixtureCCountedModePointer m) { ++g_modes; delete m.m_pObject; return true; } };
struct FixtureCBaseIntelligentPointer { FixtureCThing* m_pItem; FixtureCThing* GetPItem() const { return m_pItem; } };
struct FixtureCIntelligentPointerCreature : FixtureCBaseIntelligentPointer { FixtureCThing* GetCreature() const { return GetPItem(); } };
inline bool IS_THING_ALIVE(const FixtureCBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class FixtureCCombatActionBase { public:
    FixtureCBaseIntelligentPointer PCreature;
    void SetCombatAnimation(const FixtureCCharString& n) { std::strcpy(g_anim, n.s); }
    void Initialise() { ++g_baseInit; }
    FixtureCThing* GetCreature() const { return PCreature.GetPItem(); }
};
class FixtureCCreatureAction_BriarRoseConcentrateInto : public FixtureCCombatActionBase { public: bool Imposter; FixtureCIntelligentPointerCreature PEnemy; void Initialise(); };

void FixtureCCreatureAction_BriarRoseConcentrateInto::Initialise()
{
    FixtureCTCCreatureModeManager* modes = 0;
    GetCreature()->UnsheathePrimaryWeapon();
    SetCombatAnimation("POWER_UP_INTO");
    FixtureCCombatActionBase::Initialise();
    if (GetCreature()->GetInterface(&modes, 0x31) && IS_THING_ALIVE(PEnemy))
        modes->AddMode(FixtureCCountedModePointer(new FixtureCModeBriarRoseConcentrate(GetCreature(), PEnemy.GetCreature(), Imposter)));
    FixtureCDefPointer<FixtureCSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = FixtureCDefClassReference(GetCreature()->m_pDef)->GetSubDef(effects) &&
                 effects.m_p->GetEffectFromString("TELEPORT_INTO", effectType);
    if (found)
        CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
}

int main()
{
    int ok = 1;
    FixtureCSpecialEffectsDef fx; fx.m_refCount = 1; fx.Destroyed = 0; fx.Known = "TELEPORT_INTO"; fx.KnownType = 17;
    FixtureCDefClassBase def; def.m_refCount = 1; def.Destroyed = 0; def.Effects = &fx;
    FixtureCTCCreatureModeManager modes;
    FixtureCThing rose; rose.m_flags91 = 0; rose.m_pDef = &def; rose.Pos.X = 1; rose.Modes = &modes;
    FixtureCThing enemy; enemy.m_flags91 = 0;
    FixtureCCreatureAction_BriarRoseConcentrateInto a; a.PCreature.m_pItem = &rose; a.PEnemy.m_pItem = &enemy; a.Imposter = true;
    a.Initialise();
    if (g_unsheathes != 1 || std::strcmp(g_anim, "POWER_UP_INTO") != 0 || g_baseInit != 1) ok = 0;
    if (g_modes != 1 || g_modeCreature != &rose || g_modeEnemy != &enemy || !g_modeImposter) ok = 0;
    if (g_effects != 1 || g_effectType != 17 || g_effectPos != &rose.Pos || fx.m_refCount != 1 || def.m_refCount != 1) ok = 0;
    // dead enemy: no mode; unknown effect name: no effect; references balanced
    enemy.m_flags91 = 1; fx.Known = "OTHER";
    a.Initialise();
    if (g_modes != 1 || g_effects != 1 || fx.m_refCount != 1 || def.m_refCount != 1 || fx.Destroyed || def.Destroyed) ok = 0;
    // no effects sub-def and no mode manager
    def.Effects = 0; rose.Modes = 0; enemy.m_flags91 = 0;
    a.Initialise();
    if (g_modes != 1 || g_effects != 1 || g_baseInit != 3) ok = 0;
    std::printf("008c1c30_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}