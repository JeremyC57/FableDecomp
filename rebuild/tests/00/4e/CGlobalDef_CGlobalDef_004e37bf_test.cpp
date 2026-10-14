#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_order[16]; static long g_n = 0;
static void Log(void* p) { g_order[g_n++] = p; }
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; ~CBaseIntelligentPointer() { Log(this); } };
struct CDefMember { unsigned char _body[12]; ~CDefMember() { Log(this); } };
struct CDefBase { unsigned char _body[76]; ~CDefBase() { Log(this); } };
struct CGlobalDef : CDefBase {
    CBaseIntelligentPointer m_pointer4c; unsigned char _pad54[0x14]; CDefMember m_member68; CDefMember m_member74; unsigned char _pad80[0x54];
    CBaseIntelligentPointer m_pointerd4; CBaseIntelligentPointer m_pointerdc; CBaseIntelligentPointer m_pointere4; CBaseIntelligentPointer m_pointerec;
    ~CGlobalDef() { }
};

int main() {
    void* expect[16]; { CGlobalDef d; void* e[] = { (void*)&d.m_pointerec, (void*)&d.m_pointere4, (void*)&d.m_pointerdc, (void*)&d.m_pointerd4, (void*)&d.m_member74, (void*)&d.m_member68, (void*)&d.m_pointer4c, (void*)(CDefBase*)&d }; std::memcpy(expect, e, sizeof e); } if (g_n != 8) { std::printf("004e37bf_TEST FAIL\n"); return 1; } for (int i = 0; i < g_n; ++i) if (g_order[i] != expect[i]) { std::printf("004e37bf_TEST FAIL\n"); return 1; }
    std::printf("004e37bf_TEST PASS\n"); return 0;
}