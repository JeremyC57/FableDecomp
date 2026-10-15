// Behaviour test for 0x007813D0 CFirePoint copy constructor.
#include <cstdio>
struct CThing { int id; };
static int g_strCopies = 0;
struct CCharString { const char* s; CCharString() : s(0) {} CCharString(const CCharString& o) : s(o.s) { ++g_strCopies; } };
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) {}
    CIntelligentPointer(const CIntelligentPointer& o) : m_pItem(0) { SetPItem(o.GetPItem()); }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* t) { m_pItem = t; }
};
struct CFirePoint {
    CCharString HelperName; int HelperIndex, EffectIndex, Resistance; bool Burning; int FireDamageAfterFrame;
    CThing* PParentThing; CIntelligentPointer PEffect, PSingeEffect;
    CFirePoint() {}
    CFirePoint(const CFirePoint& other)
        : HelperName(other.HelperName), HelperIndex(other.HelperIndex), EffectIndex(other.EffectIndex),
          Resistance(other.Resistance), Burning(other.Burning), FireDamageAfterFrame(other.FireDamageAfterFrame),
          PParentThing(other.PParentThing), PEffect(other.PEffect), PSingeEffect(other.PSingeEffect) {}
};
int main()
{
    CThing parent, fx, singe;
    CFirePoint a; a.HelperName.s = "fire"; a.HelperIndex = 1; a.EffectIndex = 2; a.Resistance = 3; a.Burning = true;
    a.FireDamageAfterFrame = 4; a.PParentThing = &parent; a.PEffect.SetPItem(&fx); a.PSingeEffect.SetPItem(&singe);
    CFirePoint b(a);
    if (g_strCopies != 1 || b.HelperName.s != a.HelperName.s || b.HelperIndex != 1 || b.EffectIndex != 2 || b.Resistance != 3 ||
        !b.Burning || b.FireDamageAfterFrame != 4 || b.PParentThing != &parent || b.PEffect.GetPItem() != &fx || b.PSingeEffect.GetPItem() != &singe) {
        std::printf("007813d0_TEST FAIL\n"); return 1;
    }
    std::printf("007813d0_TEST PASS\n");
    return 0;
}
