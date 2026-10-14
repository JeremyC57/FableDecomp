#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_sections[2]; static bool g_marked = false; struct CLockedFlag { unsigned char _pad[0x14]; void* m_pSection; unsigned char _pad18[0x7d]; bool m_marked; }; static CLockedFlag* g_obj = 0;
static void Enter(void* s) { g_sections[0] = s; std::strcat(g_seq, g_obj->m_marked ? "E1" : "E0"); } static void Leave(void* s) { g_sections[1] = s; std::strcat(g_seq, g_obj->m_marked ? "L1" : "L0"); }
static long MarkUnderLock_00ca8e10(CLockedFlag* object) { void* section = object->m_pSection; Enter(section); object->m_marked = true; Leave(section); return 0; }

int main() {
    CLockedFlag o; int section; std::memset(&o, 0, sizeof o); o.m_pSection = &section; g_obj = &o; if (MarkUnderLock_00ca8e10(&o) != 0 || !o.m_marked || std::strcmp(g_seq, "E0L1") || g_sections[0] != &section || g_sections[1] != &section) { std::printf("00ca8e10_TEST FAIL\n"); return 1; }
    std::printf("00ca8e10_TEST PASS\n"); return 0;
}