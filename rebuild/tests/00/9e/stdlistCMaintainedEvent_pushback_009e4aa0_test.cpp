#include <cstdio>
static long g_inserted = 0; static int g_copies = 0;
struct CKey_009e4aa0 { long m_value; CKey_009e4aa0(long v) : m_value(v) { } CKey_009e4aa0(const CKey_009e4aa0& o) : m_value(o.m_value) { ++g_copies; } };
struct CHandle_009e4aa0 { void* m_pNode; CHandle_009e4aa0(void* n) : m_pNode(n) { } };
struct CKeyedOwner_009e4aa0 {
    CKey_009e4aa0 m_key;
    CKeyedOwner_009e4aa0(long v) : m_key(v) { }
    CHandle_009e4aa0 Insert(CKey_009e4aa0 key) { g_inserted = key.m_value; return CHandle_009e4aa0(this); }
    void Register() { Insert(m_key); }
};
int main()
{
    CKeyedOwner_009e4aa0 o(99);
    o.Register();
    if (g_inserted != 99 || g_copies < 1 || sizeof(CKey_009e4aa0) != 4 || sizeof(CHandle_009e4aa0) != 4) { std::printf("009e4aa0_TEST FAIL\n"); return 1; }
    std::printf("009e4aa0_TEST PASS\n"); return 0;
}