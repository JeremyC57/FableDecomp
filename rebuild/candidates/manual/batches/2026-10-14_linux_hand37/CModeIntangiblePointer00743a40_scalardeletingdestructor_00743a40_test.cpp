#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
struct CBaseIntelligentPointer { unsigned char _body[8]; ~CBaseIntelligentPointer() { g_seq[g_n++] = 'm'; } };
struct CModeIntangible { virtual void v00() { } unsigned char _pad[148]; ~CModeIntangible() { g_seq[g_n++] = 'b'; } };
struct CModeIntangiblePointer_00743a40 { CModeIntangible m_base; CBaseIntelligentPointer m_member; ~CModeIntangiblePointer_00743a40() { } };
static void Destroy_00743a40(CModeIntangiblePointer_00743a40* o) { delete o; }

int main() {
    CModeIntangiblePointer_00743a40* o = new CModeIntangiblePointer_00743a40; Destroy_00743a40(o); if (std::strcmp(g_seq, "mb")) { std::printf("00743a40_TEST FAIL\n"); return 1; }
    std::printf("00743a40_TEST PASS\n"); return 0;
}
