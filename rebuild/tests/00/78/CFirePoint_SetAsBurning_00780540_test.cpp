// Behaviour test for 0x00780540 CFirePoint::SetAsBurning.
#include <cstdio>
class CThing;
class C3DVector { public: float x, y, z; };
class CCharString { public: const char* s; };
static int g_attaches = 0; static CThing* g_attachTo = 0; static int g_attachType = 0, g_attachIndex = 0;
static const CCharString* g_attachName = 0;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& t_ref, int type, const CCharString& name, int index, float)
    { CThing* t = &t_ref; ++g_attaches; g_attachTo = t; g_attachType = type; g_attachName = &name; g_attachIndex = index; }
};
class CThing { public:
    unsigned int m_interfaceFlags; CTCDParticleEmitter* m_pParticleEmitter; unsigned char m_statusFlags; C3DVector m_pos;
    const C3DVector& GetPosition() { return m_pos; }
    CTCDParticleEmitter* GetParticleEmitter() { CTCDParticleEmitter* e = 0; if (m_interfaceFlags & 0x800) e = m_pParticleEmitter; return e; }
};
static CThing* g_next = 0; static int g_type = 0; static const C3DVector* g_pos = 0;
static CThing* CreateEffectThing(int type, const C3DVector* pos, bool) { g_type = type; g_pos = pos; return g_next; }
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) {}
    CIntelligentPointer(CThing* t) : m_pItem(t) {}
    CThing* GetPItem() const { return m_pItem; }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { m_pItem = o.GetPItem(); return *this; }
};
struct CFirePoint {
    CCharString HelperName; int HelperIndex, EffectIndex, Resistance; bool Burning; int FireDamageAfterFrame;
    CThing* PParentThing; CIntelligentPointer PEffect, PSingeEffect;
    void SetAsBurning()
    {
        Burning = true;
        if (EffectIndex) {
            CThing* effect = CreateEffectThing(EffectIndex, &PParentThing->GetPosition(), false);
            if (effect && !(effect->m_statusFlags & 1)) {
                effect->GetParticleEmitter()->AttachToThing(*PParentThing, 0xc, HelperName, HelperIndex, 0.0f);
                PEffect = CIntelligentPointer(effect);
            }
        }
    }
};
static int Fail() { std::printf("00780540_TEST FAIL\n"); return 1; }
int main()
{
    CTCDParticleEmitter em; CThing parent; parent.m_interfaceFlags = 0;
    CThing fx; fx.m_interfaceFlags = 0x800; fx.m_pParticleEmitter = &em; fx.m_statusFlags = 0;
    CFirePoint p; p.HelperName.s = "h"; p.HelperIndex = 3; p.EffectIndex = 0; p.Burning = false; p.PParentThing = &parent;
    p.SetAsBurning();
    if (!p.Burning || g_type != 0 || p.PEffect.m_pItem) return Fail();
    p.EffectIndex = 42; g_next = 0; p.SetAsBurning();
    if (g_type != 42 || g_pos != &parent.m_pos || g_attaches != 0 || p.PEffect.m_pItem) return Fail();
    fx.m_statusFlags = 1; g_next = &fx; p.SetAsBurning();
    if (g_attaches != 0 || p.PEffect.m_pItem) return Fail();
    fx.m_statusFlags = 0; p.SetAsBurning();
    if (g_attaches != 1 || g_attachTo != &parent || g_attachType != 0xc || g_attachName != &p.HelperName || g_attachIndex != 3 || p.PEffect.m_pItem != &fx) return Fail();
    std::printf("00780540_TEST PASS\n");
    return 0;
}