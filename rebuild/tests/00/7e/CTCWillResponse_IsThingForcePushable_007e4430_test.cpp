#include <cstdio>
// Shared test model: a thing with an interface presence bitset and a sorted {kind, component} map searched by lower bound
// (the retail CThing::PeekTC / GetTC shape), and the CTCWillResponse response bits.
struct STCEntry { long m_kind; void* m_pTC; };
struct CThing {
    unsigned long m_tcMask[9];
    STCEntry      m_entries[8];
    int           m_count;
    CThing() : m_count(0) { for (int i = 0; i < 9; ++i) m_tcMask[i] = 0; }
    bool HasTC(long kind) const { return (m_tcMask[kind >> 5] & (1UL << (kind & 31))) != 0; }
    void Add(long kind, void* tc)
    {
        int i = m_count++;
        while (i > 0 && m_entries[i - 1].m_kind > kind) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].m_kind = kind; m_entries[i].m_pTC = tc;
        m_tcMask[kind >> 5] |= 1UL << (kind & 31);
    }
    void Remove(long kind)
    {
        int j = 0;
        for (int i = 0; i < m_count; ++i) if (m_entries[i].m_kind != kind) m_entries[j++] = m_entries[i];
        m_count = j;
        m_tcMask[kind >> 5] &= ~(1UL << (kind & 31));
    }
    void* Find(long kind) const
    {
        const STCEntry* it = m_entries; int len = m_count;
        while (len > 0) { int half = len / 2; const STCEntry* mid = it + half; if (mid->m_kind < kind) { it = mid + 1; len = len - half - 1; } else len = half; }
        return it->m_pTC;
    }
    template <class T> bool PeekTC(const T** out) const
    {
        if (HasTC(T::INTERFACE_TYPE)) { *out = (const T*)Find(T::INTERFACE_TYPE); return true; }
        return false;
    }
};
struct CTCWillResponse {
    enum { INTERFACE_TYPE = 0x9c };
    bool IsForcePushable : 1;
    bool IsLifeDrainable : 1;
    bool IsLightningable : 1;
    bool IsNotBloodyWillable : 1;
    bool IsAbleToBeLifted : 1;
    bool IsEpicPowerExplodable : 1;
    bool IsEpicSpellable : 1;
    CTCWillResponse() : IsForcePushable(true), IsLifeDrainable(true), IsLightningable(true), IsNotBloodyWillable(false),
        IsAbleToBeLifted(true), IsEpicPowerExplodable(true), IsEpicSpellable(true) { }
};

struct CTCPhysicalShieldSpell { enum { INTERFACE_TYPE = 0x70 }; };
struct CTCCreatureModeManager {
    enum { INTERFACE_TYPE = 0x4a };
    int ActiveMode;
    bool IsModeActive(int mode) const { return ActiveMode == mode; }
};
static bool IsThingForcePushable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc)) {
        bool pushable = tc->IsNotBloodyWillable ? false : tc->IsForcePushable;
        if (!pushable) return false;
    }
    if (thing->HasTC(CTCPhysicalShieldSpell::INTERFACE_TYPE)) return false;
    const CTCCreatureModeManager* modes;
    if (thing->PeekTC(&modes) && modes->IsModeActive(0x36)) return false;
    return true;
}
int main()
{
    CThing bare; if (!IsThingForcePushable(&bare)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    CThing t; CTCWillResponse w; CTCCreatureModeManager m; m.ActiveMode = 0; t.Add(0x9c, &w); t.Add(0x4a, &m);
    if (!IsThingForcePushable(&t)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    w.IsForcePushable = false; if (IsThingForcePushable(&t)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    w.IsForcePushable = true; w.IsNotBloodyWillable = true; if (IsThingForcePushable(&t)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    w.IsNotBloodyWillable = false; m.ActiveMode = 0x36; if (IsThingForcePushable(&t)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    m.ActiveMode = 1; int shield = 0; t.Add(0x70, &shield); if (IsThingForcePushable(&t)) { std::printf("007e4430_TEST FAIL\n"); return 1; }
    std::printf("007e4430_TEST PASS\n");
    return 0;
}