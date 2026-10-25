// Behaviour test for 00c1e800: CBlendOwner::RemoveAllBlends -- remove each list's last entry through RemoveBlend until the unkeyed list, then the keyed list, is empty
#include <cstdio>

struct CBlendEntry { int m_keyed; };
struct CBlendArray { CBlendEntry** m_data; unsigned int m_capacity; unsigned int m_count; };

struct CBlendOwner {
    CBlendArray m_unkeyed;
    CBlendArray m_keyed;
    CBlendEntry* m_log[8]; int m_logged;
    void RemoveBlend(CBlendEntry* entry)
    {
        m_log[m_logged++] = entry;
        CBlendArray& a = entry->m_keyed ? m_keyed : m_unkeyed;
        for (unsigned int i = 0; i < a.m_count; ++i)
            if (a.m_data[i] == entry) { for (unsigned int j = i; j + 1 < a.m_count; ++j) a.m_data[j] = a.m_data[j + 1]; --a.m_count; break; }
    }
    void RemoveAllBlends()
    {
        while (m_unkeyed.m_count)
            RemoveBlend(m_unkeyed.m_data[m_unkeyed.m_count - 1]);
        while (m_keyed.m_count)
            RemoveBlend(m_keyed.m_data[m_keyed.m_count - 1]);
    }
};

int main()
{
    CBlendEntry u1 = { 0 }, u2 = { 0 }, k1 = { 1 }, k2 = { 1 }, k3 = { 1 };
    CBlendEntry* us[2] = { &u1, &u2 }; CBlendEntry* ks[3] = { &k1, &k2, &k3 };
    CBlendOwner o; o.m_logged = 0;
    o.m_unkeyed.m_data = us; o.m_unkeyed.m_capacity = 2; o.m_unkeyed.m_count = 2;
    o.m_keyed.m_data = ks; o.m_keyed.m_capacity = 3; o.m_keyed.m_count = 3;
    o.RemoveAllBlends();
    CBlendEntry* want[5] = { &u2, &u1, &k3, &k2, &k1 };
    if (o.m_logged != 5 || o.m_unkeyed.m_count != 0 || o.m_keyed.m_count != 0) { std::printf("00c1e800_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (o.m_log[i] != want[i]) { std::printf("00c1e800_TEST FAIL\n"); return 1; }
    o.RemoveAllBlends();
    if (o.m_logged != 5) { std::printf("00c1e800_TEST FAIL\n"); return 1; }
    std::printf("00c1e800_TEST PASS\n");
    return 0;
}