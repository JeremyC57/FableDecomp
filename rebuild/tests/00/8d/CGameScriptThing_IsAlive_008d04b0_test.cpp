// Behaviour test for CGameScriptThing::IsAlive (0x008D04B0), linked against the row object: true only while the intelligent pointer at
// +0xc (PThing) yields a thing whose dead bit (bit 0 of +0x91) is clear; GetPItem is called on that member, twice when alive.
#include <cstdio>
#include <cstring>
class CBaseObject;
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
class CGameScriptThing { public: virtual bool IsAlive() const; public: static bool Call(const CGameScriptThing* p) { return p->CGameScriptThing::IsAlive(); } };
static double g_object[0x20];
static unsigned char g_thing[0x98];
static CBaseObject* g_item;
static int g_calls;
static const void* g_this;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_calls; g_this = this; return g_item; }
int main()
{
    const CGameScriptThing* o = (const CGameScriptThing*)g_object;
    const unsigned char* base = (const unsigned char*)g_object;
    std::memset(g_thing, 0, sizeof g_thing);
    g_item = (CBaseObject*)g_thing; g_calls = 0;
    if (!CGameScriptThing::Call(o) || g_calls != 2 || g_this != base + 0xc) { std::printf("008d04b0_TEST FAIL alive\n"); return 1; }
    g_thing[0x91] = 1; g_calls = 0;
    if (CGameScriptThing::Call(o) || g_calls != 2) { std::printf("008d04b0_TEST FAIL dead\n"); return 1; }
    g_thing[0x91] = 0xFE; g_calls = 0;
    if (!CGameScriptThing::Call(o)) { std::printf("008d04b0_TEST FAIL bit0\n"); return 1; }
    g_item = 0; g_calls = 0;
    if (CGameScriptThing::Call(o) || g_calls != 1) { std::printf("008d04b0_TEST FAIL null\n"); return 1; }
    std::printf("008d04b0_TEST PASS\n");
    return 0;
}
