#include <cstdio>
static long g_inserted = 0; static int g_copies = 0;
struct CKey_007467d0 { long m_value; CKey_007467d0(long v) : m_value(v) { } CKey_007467d0(const CKey_007467d0& o) : m_value(o.m_value) { ++g_copies; } };
struct CHandle_007467d0 { void* m_pNode; CHandle_007467d0(void* n) : m_pNode(n) { } };
struct CKeyedOwner_007467d0 {
    CKey_007467d0 m_key;
    CKeyedOwner_007467d0(long v) : m_key(v) { }
    CHandle_007467d0 Insert(CKey_007467d0 key) { g_inserted = key.m_value; return CHandle_007467d0(this); }
    void Register() { Insert(m_key); }
};
int main()
{
    CKeyedOwner_007467d0 o(99);
    o.Register();
    if (g_inserted != 99 || g_copies < 1 || sizeof(CKey_007467d0) != 4 || sizeof(CHandle_007467d0) != 4) { std::printf("007467d0_TEST FAIL\n"); return 1; }
    std::printf("007467d0_TEST PASS\n"); return 0;
}
