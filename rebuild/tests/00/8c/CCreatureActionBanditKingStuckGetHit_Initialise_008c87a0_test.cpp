// Behaviour test for CCreatureAction_BanditKingStuckGetHit::Initialise (0x008C87A0): leaves the interface-9 flag alone, sets
// STUCK_GET_HIT, creates STUCKLOOP at the creature's position, keeps it in PStuckEffect, attaches its emitter to the creature
// at bone 0x2A "", runs the base Initialise; a missing effect name skips the create and attach.
#include <cstdio>
#include <cstring>
#include <string>

class C3DVector { public: float X, Y, Z; };
class CCharString { public: std::string s; CCharString(const char* t) : s(t) {} };
struct CTCInterface9 { bool m_flag10; };
class CThing;
static std::string g_attachBone; static CThing* g_attachThing = 0; static long g_attachIndex = -1; static int g_attaches = 0;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long index, const CCharString& bone, long, long) { g_attachThing = thing; g_attachIndex = index; g_attachBone = bone.s; ++g_attaches; }
};
class CSpecialEffectsDef { public:
    long RefCount; bool Destroyed; const char* Known; long KnownValue;
    void Destroy() { Destroyed = true; }
    bool CalcAndLookup(const CCharString& name, long* value) { if (name.s != Known) return false; *value = KnownValue; return true; }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
class CDefClassBase { public:
    CSpecialEffectsDef* Effects;
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out) { if (!Effects) return false; ++Effects->RefCount; out->Object = Effects; return true; }
};
class CThing { public:
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
class CCombatActionBase { public:
    CIntelligentPointerThing PCreature; std::string Anim; bool BaseInit;
    void SetAnimationName(const CCharString& n) { Anim = n.s; }
    void Initialise() { BaseInit = true; }
};
class CCreatureAction_BanditKingStuckGetHit : public CCombatActionBase { public: CIntelligentPointerThing PStuckEffect; void Initialise(); };

void CCreatureAction_BanditKingStuckGetHit::Initialise()
{
    SetAnimationName("STUCK_GET_HIT");
    CDefPointer<CSpecialEffectsDef> effects;
    bool found;
    {
        long effect = 0;
        found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("STUCKLOOP", &effect);
        if (found)
            PStuckEffect = CIntelligentPointerThing(CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false));
    }
    if (found && PStuckEffect.GetPItem() && !PStuckEffect.GetPItem()->Dead) {
        CTCDParticleEmitter* emitter = 0;
        PStuckEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(PCreature.GetPItem(), 0x2a, "", 0, 0);
    }
    CCombatActionBase::Initialise();
}

int main()
{
    CTCDParticleEmitter emitter; CTCInterface9 i9; i9.m_flag10 = true;
    CSpecialEffectsDef fx; fx.RefCount = 1; fx.Destroyed = false; fx.Known = "STUCKLOOP"; fx.KnownValue = 77;
    CDefClassBase def; def.Effects = &fx;
    CThing creature; creature.Pos.X = 1; creature.Pos.Y = 2; creature.Pos.Z = 3; creature.Def = &def; creature.Dead = false;
    creature.I9 = &i9; creature.Emitter = 0;
    g_effect.Dead = false; g_effect.Emitter = &emitter; g_effect.I9 = 0;
    CCreatureAction_BanditKingStuckGetHit a; a.PCreature = CIntelligentPointerThing(&creature); a.BaseInit = false;
    a.Initialise();
    bool ok = i9.m_flag10 && a.Anim == "STUCK_GET_HIT" && a.BaseInit && g_creates == 1 && g_effect.EffectId == 77
        && g_createdAt.Y == 2 && a.PStuckEffect.GetPItem() == &g_effect && g_attaches == 1 && g_attachThing == &creature
        && g_attachIndex == 0x2a && g_attachBone == "" && fx.RefCount == 1 && !fx.Destroyed;
    fx.Known = "OTHER";
    CCreatureAction_BanditKingStuckGetHit b; b.PCreature = CIntelligentPointerThing(&creature); b.BaseInit = false;
    b.Initialise();
    ok = ok && b.BaseInit && g_creates == 1 && g_attaches == 1 && b.PStuckEffect.GetPItem() == 0;
    if (!ok) { std::printf("008c87a0_TEST FAIL\n"); return 1; }
    std::printf("008c87a0_TEST PASS\n");
    return 0;
}