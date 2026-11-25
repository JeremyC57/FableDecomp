// Behaviour test for CTCKraken::HeadReadyToStrike (0x007FB120), linked against the row object: true only while the intelligent
// pointer at +0x20 (PTarget) yields a thing whose dead bit (bit 0 of +0x91) is clear.
#include <cstdio>
#include <cstring>
class CBaseObject;
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
class CTCKraken { public: bool HeadReadyToStrike(); };
static unsigned char g_kraken[0x68];
static unsigned char g_thing[0x98];
static CBaseObject* g_item;
static int g_calls;
static const void* g_this;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_calls; g_this = this; return g_item; }
int main()
{
    CTCKraken* k = (CTCKraken*)g_kraken;
    std::memset(g_thing, 0, sizeof g_thing);
    g_item = (CBaseObject*)g_thing; g_calls = 0;
    if (!k->HeadReadyToStrike() || g_calls != 2 || g_this != g_kraken + 0x20) { std::printf("007fb120_TEST FAIL alive\n"); return 1; }
    g_thing[0x91] = 1; g_calls = 0;
    if (k->HeadReadyToStrike() || g_calls != 2) { std::printf("007fb120_TEST FAIL dead\n"); return 1; }
    g_thing[0x91] = 0xFE; g_calls = 0;
    if (!k->HeadReadyToStrike()) { std::printf("007fb120_TEST FAIL bit0\n"); return 1; }
    g_item = 0; g_calls = 0;
    if (k->HeadReadyToStrike() || g_calls != 1) { std::printf("007fb120_TEST FAIL null\n"); return 1; }
    std::printf("007fb120_TEST PASS\n");
    return 0;
}
