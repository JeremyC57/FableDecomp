#include <cstdio>
static int g_converted = 0, g_destroyed = 0;
struct CCharString { void* m_pData; CCharString(void* d) : m_pData(d) {} CCharString(const CCharString& o) : m_pData(o.m_pData) {} ~CCharString() { ++g_destroyed; } operator const char*() const { ++g_converted; return "x"; } };
struct CNamedHandle_009d1e70 { void* m_pObject; CNamedHandle_009d1e70(CCharString name, void* object); };
CNamedHandle_009d1e70::CNamedHandle_009d1e70(CCharString name, void* object) { const char* text = name; m_pObject = object; (void)text; }
int main()
{
    int obj; CCharString n(&obj);
    { CNamedHandle_009d1e70 h(n, &obj); if (h.m_pObject != &obj || g_converted != 1) { std::printf("009d1e70_TEST FAIL\n"); return 1; } }
    if (g_destroyed != 1) { std::printf("009d1e70_TEST FAIL\n"); return 1; }
    std::printf("009d1e70_TEST PASS\n");
    return 0;
}