// Behaviour test for 0x0080E5D0 ForEachIf<CThing**, CThingFilter_IsThrownByExplosion, CApplyAdditionalExplosiveImpulseIfInsideRing>:
// the functor runs only for live things that own component 0x31 whose current mode has type 0x42; null, dead, component-less
// and other-mode things are skipped.
#include <cstdio>
class CTCBase;
class CMode {
public:
    long Type;
    virtual void v00() {}
    virtual long GetModeType() { return Type; }
};
class CTCCreatureModeManager {
public:
    CMode* Mode;
    CMode* GetCurrentMode() { return Mode; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> T* GetTC(long type) const
    {
        CInterfaceMap::SEntry* last = m_interfaces.last;
        CInterfaceMap::SEntry* first = m_interfaces.first;
        int count = last - first;
        CInterfaceMap::SEntry* it = first;
        while (count > 0) {
            int half = count / 2;
            if (it[half].type < type) { it += half + 1; count -= half + 1; }
            else count = half;
        }
        if (it == last || type < it->type)
            it = last;
        return (T*)it->component;
    }
};
namespace {
struct CThingFilter_IsThrownByExplosion {
    bool operator()(CThing* thing) const
    {
        if (thing && !thing->Dead && thing->HasTC(0x31)
            && thing->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetModeType() == 0x42)
            return true;
        return false;
    }
};
}
static int g_hits = 0; static CThing* g_last = 0;
class CApplyAdditionalExplosiveImpulseIfInsideRing { public:
    float m_data[7];
    void operator()(CThing* thing) const { ++g_hits; g_last = thing; }
};
template <class It, class Pred, class Func> void __fastcall ForEachIf(It first, It last, Pred pred, Func func)
{
    for (; first != last; ++first)
        if (pred(*first))
            func(*first);
}
static void SetTC(CThing& t, CInterfaceMap::SEntry* entries, int n, CTCCreatureModeManager* mgr)
{
    // entries sorted by type: {0x10, 0}, {0x31, mgr}, {0x40, 0}
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x31; entries[1].component = (CTCBase*)mgr;
    entries[2].type = 0x40; entries[2].component = 0;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + n;
    t.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
}
int main()
{
    CMode thrown; thrown.Type = 0x42; CMode walking; walking.Type = 0x07;
    CTCCreatureModeManager mThrown; mThrown.Mode = &thrown; CTCCreatureModeManager mWalk; mWalk.Mode = &walking;
    CThing a, b, c, d; CInterfaceMap::SEntry ea[3], eb[3], ec[3];
    int i = 0;
    for (; i < 9; ++i) { a.m_presentBits[i] = b.m_presentBits[i] = c.m_presentBits[i] = d.m_presentBits[i] = 0; }
    a.Dead = 0; b.Dead = 1; c.Dead = 0; d.Dead = 0;
    SetTC(a, ea, 3, &mThrown);          // live, thrown        -> hit
    SetTC(b, eb, 3, &mThrown);          // dead                -> skipped
    SetTC(c, ec, 3, &mWalk);            // live, other mode    -> skipped
    d.m_interfaces.first = d.m_interfaces.last = 0;   // no component 0x31 -> skipped (GetTC never reached)
    CThing* things[5] = { &d, &b, 0, &a, &c };
    CApplyAdditionalExplosiveImpulseIfInsideRing func = { { 1, 2, 3, 4, 5, 6, 7 } };
    ForEachIf(things, things + 5, CThingFilter_IsThrownByExplosion(), func);
    bool ok = g_hits == 1 && g_last == &a;
    g_hits = 0;
    ForEachIf(things, things, CThingFilter_IsThrownByExplosion(), func);   // empty range
    ok = ok && g_hits == 0;
    if (!ok) { std::printf("0080e5d0_TEST FAIL\n"); return 1; }
    std::printf("0080e5d0_TEST PASS\n");
    return 0;
}