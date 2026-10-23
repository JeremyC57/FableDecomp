// Behaviour test for CCreatureAction_BriarRoseConcentrateInto::Initialise (0x008C1C30): unsheathe, POWER_UP_INTO, base
// Initialise; a mode manager plus a live enemy adds CModeBriarRoseConcentrate(creature, enemy, Imposter); the def's
// TELEPORT_INTO effect is created at the creature when the def has effects and knows the name.
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CThing;
static int g_unsheathes = 0, g_baseInit = 0, g_modes = 0, g_effects = 0; static char g_anim[32];
static CThing* g_modeCreature = 0; static CThing* g_modeEnemy = 0; static bool g_modeImposter = false;
static long g_effectType = -1; static const C3DVector* g_effectPos = 0;
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
struct CSpecialEffectsDef {
    long m_refCount; int Destroyed; const char* Known; long KnownType;
    void Destroy() { ++Destroyed; }
    bool GetEffectFromString(const CCharString& n, long& type) const { if (std::strcmp(n.s, Known) != 0) return false; type = KnownType; return true; }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
};
struct CDefClassBase {
    long m_refCount; int Destroyed; CSpecialEffectsDef* Effects;
    void Destroy() { ++Destroyed; }
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>& out) { if (!Effects) return false; ++Effects->m_refCount; out.m_p = Effects; return true; }
};
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CTCCreatureModeManager;
struct CThing {
    unsigned char m_flags91; CDefClassBase* m_pDef; C3DVector Pos; CTCCreatureModeManager* Modes;
    const C3DVector& GetPosition() const { return Pos; }
    void UnsheatheMeleeWeapon_00666820() { ++g_unsheathes; }
    bool GetInterface(CTCCreatureModeManager** out, long) { if (!Modes) return false; *out = Modes; return true; }
};
CThing* CreateEffectThing(long type, const C3DVector* pos, bool) { g_effectType = type; g_effectPos = pos; ++g_effects; return 0; }
struct CModeBriarRoseConcentrate {
    CModeBriarRoseConcentrate(CThing* c, CThing* e, bool i) { g_modeCreature = c; g_modeEnemy = e; g_modeImposter = i; }
};
struct CCountedModePointer { CModeBriarRoseConcentrate* m_pObject; CCountedModePointer(CModeBriarRoseConcentrate* o) : m_pObject(o) {} };
struct CTCCreatureModeManager { bool AddMode(CCountedModePointer m) { ++g_modes; delete m.m_pObject; return true; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CIntelligentPointerCreature : CBaseIntelligentPointer { CThing* GetCreature() const { return GetPItem(); } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature;
    void SetAnimationName(const CCharString& n) { std::strcpy(g_anim, n.s); }
    void Initialise() { ++g_baseInit; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_BriarRoseConcentrateInto : CCombatActionBase { bool Imposter; CIntelligentPointerCreature PEnemy; void Initialise(); };

void CCreatureAction_BriarRoseConcentrateInto::Initialise()
{
    CTCCreatureModeManager* modes = 0;
    GetCreature()->UnsheatheMeleeWeapon_00666820();
    SetAnimationName("POWER_UP_INTO");
    CCombatActionBase::Initialise();
    if (GetCreature()->GetInterface(&modes, 0x31) && IS_THING_ALIVE(PEnemy))
        modes->AddMode(CCountedModePointer(new CModeBriarRoseConcentrate(GetCreature(), PEnemy.GetCreature(), Imposter)));
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = CDefClassReference(GetCreature()->m_pDef)->GetSubDef(effects) &&
                 effects.m_p->GetEffectFromString("TELEPORT_INTO", effectType);
    if (found)
        CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
}

int main()
{
    int ok = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1; fx.Destroyed = 0; fx.Known = "TELEPORT_INTO"; fx.KnownType = 17;
    CDefClassBase def; def.m_refCount = 1; def.Destroyed = 0; def.Effects = &fx;
    CTCCreatureModeManager modes;
    CThing rose; rose.m_flags91 = 0; rose.m_pDef = &def; rose.Pos.X = 1; rose.Modes = &modes;
    CThing enemy; enemy.m_flags91 = 0;
    CCreatureAction_BriarRoseConcentrateInto a; a.PCreature.m_pItem = &rose; a.PEnemy.m_pItem = &enemy; a.Imposter = true;
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