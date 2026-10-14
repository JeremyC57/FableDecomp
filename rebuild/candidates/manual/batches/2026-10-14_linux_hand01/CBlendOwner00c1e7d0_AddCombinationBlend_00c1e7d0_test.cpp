#include <cstdio>
struct CBlendKey { long m_index; long GetIndex() { return m_index; } };
struct CBlendEntry { unsigned char _pad[0x8]; CBlendKey m_key; };
struct CBlendList { unsigned char _body[0xc]; CBlendEntry* m_last; void Add(CBlendEntry* e) { m_last = e; } };
struct CBlendOwner { unsigned char _pad[0x8]; CBlendList m_unkeyed; CBlendList m_keyed;
    void AddCombinationBlend(CBlendEntry* e) { if (e->m_key.GetIndex() == -1) m_unkeyed.Add(e); else m_keyed.Add(e); } };
int main()
{
    CBlendOwner o; o.m_unkeyed.m_last = 0; o.m_keyed.m_last = 0;
    CBlendEntry a, b; a.m_key.m_index = -1; b.m_key.m_index = 5;
    o.AddCombinationBlend(&a); o.AddCombinationBlend(&b);
    if (o.m_unkeyed.m_last != &a || o.m_keyed.m_last != &b) { std::printf("00c1e7d0_TEST FAIL\n"); return 1; }
    std::printf("00c1e7d0_TEST PASS\n"); return 0;
}
