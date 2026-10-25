// Behaviour test for 00c1e780: CBlendOwner::AddBlend -- append the entry to the unkeyed list when its key index is -1, else to the keyed list, growing a full list by one slot
#include <cstdio>

struct CBlendKey { long m_index; long GetIndex() { return m_index; } };
struct CBlendEntry { long m_pad[2]; CBlendKey m_key; };

static int g_grows = 0;
template <class T>
struct CPtrArray {
    T* m_data;
    unsigned int m_capacity;
    unsigned int m_count;
    T m_store[8];
    void Grow(unsigned int slots) { ++g_grows; m_capacity += slots; }
    void Add(T value)
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

struct CBlendOwner {
    CPtrArray<CBlendEntry*> m_unkeyed;
    CPtrArray<CBlendEntry*> m_keyed;
    void AddBlend(CBlendEntry* entry)
    {
        if (entry->m_key.GetIndex() == -1)
            m_unkeyed.Add(entry);
        else
            m_keyed.Add(entry);
    }
};

int main()
{
    CBlendOwner o;
    o.m_unkeyed.m_data = o.m_unkeyed.m_store; o.m_unkeyed.m_capacity = 1; o.m_unkeyed.m_count = 0;
    o.m_keyed.m_data = o.m_keyed.m_store; o.m_keyed.m_capacity = 4; o.m_keyed.m_count = 0;
    CBlendEntry a, b, c, d;
    a.m_key.m_index = -1; b.m_key.m_index = 0; c.m_key.m_index = -1; d.m_key.m_index = 5;
    o.AddBlend(&a); o.AddBlend(&b); o.AddBlend(&c); o.AddBlend(&d);
    if (o.m_unkeyed.m_count != 2 || o.m_unkeyed.m_store[0] != &a || o.m_unkeyed.m_store[1] != &c) { std::printf("00c1e780_TEST FAIL\n"); return 1; }
    if (o.m_keyed.m_count != 2 || o.m_keyed.m_store[0] != &b || o.m_keyed.m_store[1] != &d) { std::printf("00c1e780_TEST FAIL\n"); return 1; }
    if (g_grows != 1 || o.m_unkeyed.m_capacity != 2 || o.m_keyed.m_capacity != 4) { std::printf("00c1e780_TEST FAIL\n"); return 1; }
    std::printf("00c1e780_TEST PASS\n");
    return 0;
}