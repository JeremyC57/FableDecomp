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

static CTCWillResponse g_pool[4];
static int g_added = 0;
static CTCWillResponse* AddTC(CThing* thing)
{
    CTCWillResponse* tc = &g_pool[g_added++];
    thing->Add(0x9c, tc);
    return tc;
}
static void Cleanup(CThing* thing, CTCWillResponse* tc)
{
    if (tc->IsForcePushable && tc->IsLifeDrainable && tc->IsLightningable && tc->IsAbleToBeLifted && tc->IsEpicPowerExplodable &&
        tc->IsEpicSpellable && !tc->IsNotBloodyWillable)
        thing->Remove(0x9c);
}
static void SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable)
{
    if (notWillable) {
        CTCWillResponse* tc;
        if (thing->HasTC(0x9c)) tc = (CTCWillResponse*)thing->Find(0x9c);
        else tc = AddTC(thing);
        tc->IsNotBloodyWillable = notWillable;
    } else if (thing->HasTC(0x9c)) {
        CTCWillResponse* tc = (CTCWillResponse*)thing->Find(0x9c);
        tc->IsNotBloodyWillable = false;
        Cleanup(thing, tc);
    }
}
int main()
{
    CThing t;
    SetThingAsNotBloodyWillableAtAll(&t, false); if (t.HasTC(0x9c) || g_added != 0) { std::printf("007e4040_TEST FAIL\n"); return 1; }
    SetThingAsNotBloodyWillableAtAll(&t, true); if (!t.HasTC(0x9c) || g_added != 1 || !g_pool[0].IsNotBloodyWillable) { std::printf("007e4040_TEST FAIL\n"); return 1; }
    SetThingAsNotBloodyWillableAtAll(&t, true); if (g_added != 1) { std::printf("007e4040_TEST FAIL\n"); return 1; }
    SetThingAsNotBloodyWillableAtAll(&t, false); if (t.HasTC(0x9c) || g_pool[0].IsNotBloodyWillable) { std::printf("007e4040_TEST FAIL\n"); return 1; }
    CThing u; CTCWillResponse w; w.IsEpicSpellable = false; u.Add(0x9c, &w);
    SetThingAsNotBloodyWillableAtAll(&u, true); SetThingAsNotBloodyWillableAtAll(&u, false);
    if (!u.HasTC(0x9c) || w.IsNotBloodyWillable) { std::printf("007e4040_TEST FAIL\n"); return 1; }
    std::printf("007e4040_TEST PASS\n");
    return 0;
}