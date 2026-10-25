#include <cstdio>
struct CBlendKey { long m_index; long GetIndex() { return m_index; } };
struct CBlendEntry { unsigned char _pad[0x8]; CBlendKey m_key; };
struct CBlendArray {
    CBlendEntry* m_data[4]; unsigned int m_count;
    void Remove(CBlendEntry* e)
    {
        unsigned int i = 0;
        while (i < m_count && m_data[i] != e) ++i;
        for (; i + 1 < m_count; ++i) m_data[i] = m_data[i + 1];
        --m_count;
    }
};
struct CBlendOwner { CBlendArray m_unkeyed; CBlendArray m_keyed;
    void RemoveBlend(CBlendEntry* e) { if (e->m_key.GetIndex() == -1) m_unkeyed.Remove(e); else m_keyed.Remove(e); } };
int main()
{
    CBlendEntry a, b, c; a.m_key.m_index = -1; b.m_key.m_index = 5; c.m_key.m_index = -1;
    CBlendOwner o;
    o.m_unkeyed.m_data[0] = &a; o.m_unkeyed.m_data[1] = &c; o.m_unkeyed.m_count = 2;
    o.m_keyed.m_data[0] = &b; o.m_keyed.m_count = 1;
    o.RemoveBlend(&a); o.RemoveBlend(&b);
    if (o.m_unkeyed.m_count != 1 || o.m_unkeyed.m_data[0] != &c || o.m_keyed.m_count != 0) { std::printf("00c1e7d0_TEST FAIL\n"); return 1; }
    std::printf("00c1e7d0_TEST PASS\n"); return 0;
}
