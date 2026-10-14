#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CThing { unsigned char _pad[0x6c]; unsigned long m_flags; }; static CThing g_thing; static CThing* g_item = 0;
struct CIntelligentPointer { virtual void v00() { } void* m_pItem; CThing* GetPItem() { std::strcat(g_seq, "g"); return g_item; } };
struct CThingHolder { unsigned char _pad[0x48]; CIntelligentPointer m_thing; CThing* GetPThing() { CThing* result = 0; if (m_thing.GetPItem()) { if (!(~m_thing.GetPItem()->m_flags & 0x1f)) result = m_thing.GetPItem(); } return result; } };

int main() {
    CThingHolder h; g_item = &g_thing; g_thing.m_flags = 0x1f; if (h.GetPThing() != &g_thing || std::strcmp(g_seq, "ggg")) { std::printf("0090df30_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_thing.m_flags = 0xffffffff; if (h.GetPThing() != &g_thing) { std::printf("0090df30_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_thing.m_flags = 0x1e; if (h.GetPThing() != 0 || std::strcmp(g_seq, "gg")) { std::printf("0090df30_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_item = 0; if (h.GetPThing() != 0 || std::strcmp(g_seq, "g")) { std::printf("0090df30_TEST FAIL\n"); return 1; }
    std::printf("0090df30_TEST PASS\n"); return 0;
}
