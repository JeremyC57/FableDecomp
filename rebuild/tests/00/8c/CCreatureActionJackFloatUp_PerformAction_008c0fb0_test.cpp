// Behaviour test for CCreatureAction_JackFloatUp::PerformAction (0x008C0FB0): FloatSpeed = FloatHeight / (duration -
// running time), Moving set; RISE_EFFECT created at Jack; FLOAT_EFFECT created, added to Jack's 0xAD dependents and, with an
// emitter (0x4B), attached to Jack at TARGET_CURSOR_POINT (type 0xC); the effects def is released.
#include <cstdio>
#include <cstring>
#include <cmath>

class C3DVector { public: float X, Y, Z; };
class CThing;
static int g_creates = 0, g_attaches = 0, g_deps = 0; static long g_types[8]; static CThing* g_dep = 0;
static CThing* g_attachTo = 0; static long g_attachType = 0; static char g_helper[32]; static long g_animAsked = -1;
class CCharString { public: const char* s; CCharString(const char* t) : s(t) {} };
class CSpecialEffectsDef { public:
    long m_refCount; int Destroyed; const char* K1; long T1; const char* K2; long T2;
    void Destroy() { ++Destroyed; }
    bool GetEffectFromString(const CCharString& n, long& type) const
    {
        if (K1 && !std::strcmp(n.s, K1)) { type = T1; return true; }
        if (K2 && !std::strcmp(n.s, K2)) { type = T2; return true; }
        return false;
    }
};
template <class T> class CDefPointer { public: T* m_p; CDefPointer() : m_p(0) { } };
class CDefClassBase { public:
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
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& t_ref, long type, const CCharString& h, long, float) { CThing* t = &t_ref; g_attachTo = t; g_attachType = type; std::strcpy(g_helper, h.s); ++g_attaches; }
};
struct CTCAD { void AddDependentThing(CThing* t) { g_dep = t; ++g_deps; } };
class CThing { public:
    unsigned char m_flags91; CDefClassBase* m_pDef; C3DVector Pos; float Duration; CTCAD* AD; CTCDParticleEmitter* PTCDriver; bool Has4B;
    const C3DVector& GetPosition() const { return Pos; }
    float GetAnimDuration(long a) { g_animAsked = a; return Duration; }
    bool HasTC(long) const { return Has4B; }
    bool GetInterface(CTCAD** o, long) { if (!AD) return false; *o = AD; return true; }
};
static CThing g_effect;
CThing* CreateEffectThing(long type, const C3DVector*, bool) { g_types[g_creates++] = type; return &g_effect; }
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public:
    CPtr PCreature; float RunningTimeSeconds; long AnimIndex;
    long GetAnimationIndex() { return AnimIndex; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_JackFloatUp : public CCombatActionBase { public: float FloatHeight, FloatSpeed; bool Moving; float StartHeight; void PerformAction(); };

void CCreatureAction_JackFloatUp::PerformAction()
{
    FloatSpeed = FloatHeight / (GetCreature()->GetAnimDuration(GetAnimationIndex()) - RunningTimeSeconds);
    Moving = true;
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassReference(GetCreature()->m_pDef)->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("RISE_EFFECT", effectType))
            CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
        if (effects->GetEffectFromString("FLOAT_EFFECT", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1)) {
                CTCAD* dependents = 0;
                GetCreature()->GetInterface(&dependents, 0xad);
                dependents->AddDependentThing(effect);
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->PTCDriver;
                    emitter->AttachToThing(*GetCreature(), 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}

int main()
{
    int ok = 1;
    CSpecialEffectsDef fx; fx.m_refCount = 1; fx.Destroyed = 0; fx.K1 = "RISE_EFFECT"; fx.T1 = 4; fx.K2 = "FLOAT_EFFECT"; fx.T2 = 9;
    CDefClassBase def; def.m_refCount = 1; def.Destroyed = 0; def.Effects = &fx;
    CTCAD ad; CTCDParticleEmitter em;
    CThing jack; jack.m_flags91 = 0; jack.m_pDef = &def; jack.Duration = 3.0f; jack.AD = &ad;
    g_effect.m_flags91 = 0; g_effect.PTCDriver = &em; g_effect.Has4B = true;
    CCreatureAction_JackFloatUp a; a.PCreature.m_pItem = &jack; a.RunningTimeSeconds = 1.0f; a.AnimIndex = 33; a.FloatHeight = 5.0f; a.Moving = false;
    a.PerformAction();
    if (std::fabs(a.FloatSpeed - 2.5f) > 1e-5f || !a.Moving || g_animAsked != 33) ok = 0;
    if (g_creates != 2 || g_types[0] != 4 || g_types[1] != 9 || g_deps != 1 || g_dep != &g_effect) ok = 0;
    if (g_attaches != 1 || g_attachTo != &jack || g_attachType != 0xc || std::strcmp(g_helper, "TARGET_CURSOR_POINT") != 0) ok = 0;
    if (fx.m_refCount != 1 || def.m_refCount != 1 || fx.Destroyed || def.Destroyed) ok = 0;
    // dead float effect: no dependents/attach; no emitter; last reference destroys the effects def
    g_effect.m_flags91 = 1; fx.m_refCount = 0;
    a.PerformAction();
    if (g_creates != 4 || g_deps != 1 || g_attaches != 1 || fx.Destroyed != 1) ok = 0;
    g_effect.m_flags91 = 0; g_effect.Has4B = false; fx.m_refCount = 1;
    a.PerformAction();
    if (g_deps != 2 || g_attaches != 1) ok = 0;
    std::printf("008c0fb0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}