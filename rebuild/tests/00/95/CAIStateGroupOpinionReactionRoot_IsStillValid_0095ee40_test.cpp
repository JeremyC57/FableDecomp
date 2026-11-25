// Behaviour test for CAIStateGroup_OpinionReactionRoot::IsStillValid (0x0095EE40), linked against the row object: true only while the intelligent pointer at
// +0x6b (ThingReactingTo) yields a thing whose dead bit (bit 0 of +0x91) is clear; GetPItem is called on that member, twice when alive.
#include <cstdio>
#include <cstring>
class CBaseObject;
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
class CAIStateGroup_OpinionReactionRoot { public: virtual bool IsStillValid() const; public: static bool Call(const CAIStateGroup_OpinionReactionRoot* p) { return p->CAIStateGroup_OpinionReactionRoot::IsStillValid(); } };
static double g_object[0x20];
static unsigned char g_thing[0x98];
static CBaseObject* g_item;
static int g_calls;
static const void* g_this;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_calls; g_this = this; return g_item; }
int main()
{
    const CAIStateGroup_OpinionReactionRoot* o = (const CAIStateGroup_OpinionReactionRoot*)g_object;
    const unsigned char* base = (const unsigned char*)g_object;
    std::memset(g_thing, 0, sizeof g_thing);
    g_item = (CBaseObject*)g_thing; g_calls = 0;
    if (!CAIStateGroup_OpinionReactionRoot::Call(o) || g_calls != 2 || g_this != base + 0x6b) { std::printf("0095ee40_TEST FAIL alive\n"); return 1; }
    g_thing[0x91] = 1; g_calls = 0;
    if (CAIStateGroup_OpinionReactionRoot::Call(o) || g_calls != 2) { std::printf("0095ee40_TEST FAIL dead\n"); return 1; }
    g_thing[0x91] = 0xFE; g_calls = 0;
    if (!CAIStateGroup_OpinionReactionRoot::Call(o)) { std::printf("0095ee40_TEST FAIL bit0\n"); return 1; }
    g_item = 0; g_calls = 0;
    if (CAIStateGroup_OpinionReactionRoot::Call(o) || g_calls != 1) { std::printf("0095ee40_TEST FAIL null\n"); return 1; }
    std::printf("0095ee40_TEST PASS\n");
    return 0;
}
