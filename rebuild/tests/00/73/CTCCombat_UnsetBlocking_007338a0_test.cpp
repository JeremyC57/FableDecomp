// Test for 0x007338A0 CTCCombat::UnsetBlocking: links the source object; the owning thing's interface map holds a
// creature-mode manager (type 0x31) whose RemoveMode records the mode; Blocking (+0x63) and StartingToBlock (+0x62)
// are cleared and the neighbouring flags are untouched.
#include <stdio.h>
#include <string.h>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
class CTCCreatureModeManager { public: void RemoveMode(unsigned long mode); };
static unsigned long g_removed = 0xffffffff;
static int g_calls;
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { g_removed = mode; ++g_calls; }
class CTCCombat { public: virtual void v00(); void UnsetBlocking(); };
void CTCCombat::v00() { }
static unsigned char g_thing[0x50];
static unsigned char g_tc[0x98];
static CTCCreatureModeManager g_modes;
static CInterfaceMap::SEntry g_entries[3];
int main()
{
    memset(g_thing, 0, sizeof(g_thing));
    unsigned long bits = 1UL << (0x31 & 31);
    memcpy(g_thing + 0x24, &bits, 4);
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x31; g_entries[1].component = (CTCBase*)&g_modes;
    g_entries[2].type = 0x40; g_entries[2].component = 0;
    CInterfaceMap map; map.first = g_entries; map.last = g_entries + 3;
    memcpy(g_thing + 0x44, &map, sizeof(map));
    CTCCombat proto;                              // copy the vtable pointer
    memset(g_tc, 0x11, sizeof(g_tc));
    memcpy(g_tc, &proto, 4);
    void* thing = g_thing; memcpy(g_tc + 4, &thing, 4);
    g_tc[0x60] = 1; g_tc[0x61] = 1; g_tc[0x62] = 1; g_tc[0x63] = 1; g_tc[0x64] = 7;
    ((CTCCombat*)g_tc)->UnsetBlocking();
    bool ok = g_calls == 1 && g_removed == 12 && g_tc[0x62] == 0 && g_tc[0x63] == 0
           && g_tc[0x60] == 1 && g_tc[0x61] == 1 && g_tc[0x64] == 7;
    if (!ok) { puts("007338a0_TEST FAIL"); return 1; }
    puts("007338a0_TEST PASS");
    return 0;
}
