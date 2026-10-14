#include <cstdio>
// Behaviour model of CRetailVector<SThreeStringEntry>::resize(unsigned int) @ 0x0056A230: builds a default
// element, forwards it with the count, and destroys its three strings afterwards.
static int g_ctor = 0, g_dtor = 0; static unsigned int g_size = 0; static bool g_sawDefault = false;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { g_ctor++; } ~CCharString() { g_dtor++; } };
struct SThreeStringEntry { CCharString m_name, m_text, m_extra; bool m_flagA, m_flagB; SThreeStringEntry() : m_name(), m_text(), m_extra(), m_flagA(false), m_flagB(false) {} };
struct CRetailVectorSThreeStringEntry { SThreeStringEntry* first; SThreeStringEntry* last; SThreeStringEntry* end_of_storage;
    void resize(unsigned int n, const SThreeStringEntry& v) { g_size = n; g_sawDefault = !v.m_flagA && !v.m_flagB && v.m_name.m_pData == 0 && v.m_extra.m_pData == 0 && g_dtor == 0; }
    void resize(unsigned int n) { resize(n, SThreeStringEntry()); } };
int main()
{
    CRetailVectorSThreeStringEntry v; v.first = v.last = v.end_of_storage = 0;
    v.resize(0x27);
    bool ok = g_size == 0x27 && g_sawDefault && g_ctor == 3 && g_dtor == 3;
    if (!ok) { std::printf("0056a230_TEST FAIL\n"); return 1; }
    std::printf("0056a230_TEST PASS\n");
    return 0;
}
