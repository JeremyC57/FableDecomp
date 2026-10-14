#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_order[16]; static long g_n = 0;
static void Log(void* p) { g_order[g_n++] = p; }
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; ~CBaseIntelligentPointer() { Log(this); } };
struct CDefMember { unsigned char _body[12]; ~CDefMember() { Log(this); } };
struct CDefBase { unsigned char _body[20]; ~CDefBase() { Log(this); } };
struct CGlobalDef : CDefBase {
    CBaseIntelligentPointer m_pointer14, m_pointer1c, m_pointer24, m_pointer2c, m_pointer34, m_pointer3c, m_pointer44;
    ~CGlobalDef() { }
};

int main() {
    void* expect[16]; { CGlobalDef d; void* e[] = { (void*)&d.m_pointer44, (void*)&d.m_pointer3c, (void*)&d.m_pointer34, (void*)&d.m_pointer2c, (void*)&d.m_pointer24, (void*)&d.m_pointer1c, (void*)&d.m_pointer14, (void*)(CDefBase*)&d }; std::memcpy(expect, e, sizeof e); } if (g_n != 8) { std::printf("009006dd_TEST FAIL\n"); return 1; } for (int i = 0; i < g_n; ++i) if (g_order[i] != expect[i]) { std::printf("009006dd_TEST FAIL\n"); return 1; }
    std::printf("009006dd_TEST PASS\n"); return 0;
}
