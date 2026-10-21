// Behaviour test for CCreatureAction_ScreamerDie::Initialise (0x008B8D30): clears the creature's interface-9 flag, sets
// ST_DIE, creates SCREAMER_DEATH at the creature's position, keeps it in PDieEffect, attaches its emitter to the creature
// at bone 0xA "DUMMY_CENTRE", runs the base Initialise; a missing effect name skips the create and attach.
#include <cstdio>
#include <cstring>
#include <string>

struct C3DVector { float X, Y, Z; };
struct CCharString { std::string s; CCharString(const char* t) : s(t) {} };
struct CTCInterface9 { bool m_flag10; };
struct CThing;
static std::string g_attachBone; static CThing* g_attachThing = 0; static long g_attachIndex = -1; static int g_attaches = 0;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long index, const CCharString& bone, long, long) { g_attachThing = thing; g_attachIndex = index; g_attachBone = bone.s; ++g_attaches; }
};
struct CSpecialEffectsDef {
    long RefCount; bool Destroyed; const char* Known; long KnownValue;
    void Destroy() { Destroyed = true; }
    bool CalcAndLookup(const CCharString& name, long* value) { if (name.s != Known) return false; *value = KnownValue; return true; }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
struct CDefClassBase {
    CSpecialEffectsDef* Effects;
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out) { if (!Effects) return false; ++Effects->RefCount; out->Object = Effects; return true; }
};
struct CThing {
    C3DVector Pos; CDefClassBase* Def; bool Dead; CTCInterface9* I9; CTCDParticleEmitter* Emitter; long EffectId;
    const C3DVector& GetPosition() const { return Pos; }
    CDefClassBase* GetDef() const { return Def; }
    bool GetInterface(CTCInterface9** out, long) { if (!I9) return false; *out = I9; return true; }
    bool GetInterface(CTCDParticleEmitter** out, long) { if (!Emitter) return false; *out = Emitter; return true; }
};
static CThing g_effect; static C3DVector g_createdAt; static int g_creates = 0;
CThing* CreateSpecialEffect(long effect, const C3DVector& pos, bool) { g_effect.EffectId = effect; g_createdAt = pos; ++g_creates; return &g_effect; }
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing() : m_pItem(0) {}
    CIntelligentPointerThing(CThing* item) : m_pItem(item) {}
    CThing* GetPItem() const { return m_pItem; }
};
struct CCombatActionBase {
    CIntelligentPointerThing PCreature; std::string Anim; bool BaseInit;
    void SetAnimationName(const CCharString& n) { Anim = n.s; }
    void Initialise() { BaseInit = true; }
};
struct CCreatureAction_ScreamerDie : CCombatActionBase { CIntelligentPointerThing PDieEffect; void Initialise(); };

void CCreatureAction_ScreamerDie::Initialise()
{
    CTCInterface9* component;
    if (PCreature.GetPItem()->GetInterface(&component, 9))
        component->m_flag10 = false;
    SetAnimationName("ST_DIE");
    CDefPointer<CSpecialEffectsDef> effects;
    bool found;
    {
        long effect = 0;
        found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("SCREAMER_DEATH", &effect);
        if (found)
            PDieEffect = CIntelligentPointerThing(CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false));
    }
    if (found && PDieEffect.GetPItem() && !PDieEffect.GetPItem()->Dead) {
        CTCDParticleEmitter* emitter = 0;
        PDieEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(PCreature.GetPItem(), 0xa, "DUMMY_CENTRE", 0, 0);
    }
    CCombatActionBase::Initialise();
}

int main()
{
    CTCDParticleEmitter emitter; CTCInterface9 i9; i9.m_flag10 = true;
    CSpecialEffectsDef fx; fx.RefCount = 1; fx.Destroyed = false; fx.Known = "SCREAMER_DEATH"; fx.KnownValue = 77;
    CDefClassBase def; def.Effects = &fx;
    CThing creature; creature.Pos.X = 1; creature.Pos.Y = 2; creature.Pos.Z = 3; creature.Def = &def; creature.Dead = false;
    creature.I9 = &i9; creature.Emitter = 0;
    g_effect.Dead = false; g_effect.Emitter = &emitter; g_effect.I9 = 0;
    CCreatureAction_ScreamerDie a; a.PCreature = CIntelligentPointerThing(&creature); a.BaseInit = false;
    a.Initialise();
    bool ok = !i9.m_flag10 && a.Anim == "ST_DIE" && a.BaseInit && g_creates == 1 && g_effect.EffectId == 77
        && g_createdAt.Y == 2 && a.PDieEffect.GetPItem() == &g_effect && g_attaches == 1 && g_attachThing == &creature
        && g_attachIndex == 0xa && g_attachBone == "DUMMY_CENTRE" && fx.RefCount == 1 && !fx.Destroyed;
    fx.Known = "OTHER";
    CCreatureAction_ScreamerDie b; b.PCreature = CIntelligentPointerThing(&creature); b.BaseInit = false;
    b.Initialise();
    ok = ok && b.BaseInit && g_creates == 1 && g_attaches == 1 && b.PDieEffect.GetPItem() == 0;
    if (!ok) { std::printf("008b8d30_TEST FAIL\n"); return 1; }
    std::printf("008b8d30_TEST PASS\n");
    return 0;
}