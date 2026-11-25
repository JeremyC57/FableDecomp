// Behaviour test for 0x0088F850 CGameScriptInterface::EntitySetDeedReactionsEnabled (NF415: linked): links the row object (the function is
// declared here, not defined) and defines its callees under the row's spellings. Checks the name built for the call is
// "SG_DEED_REACTION_ROOT" (retail 0x01275988) with length -1, that interface slot 0x938 gets (self, &name, entity, &name, enabled), and
// that the name is destroyed after the call.
#include <cstdio>
#include <cstring>
#include "engine/CGameScriptInterface.h"

struct CDeedReactionName {
    int Value;
    CDeedReactionName(int value, int length);
    ~CDeedReactionName();
};
typedef void (__fastcall *SetDeedReactionsFn)(CGameScriptInterface*, CDeedReactionName*, int, CDeedReactionName*, int);
void __fastcall CGameScriptInterface_EntitySetDeedReactionsEnabled(CGameScriptInterface* self, int, int entity, int enabled);   // the row (linked)

static int g_ctor = 0, g_dtor = 0, g_len = 0, g_calls = 0, g_entity = 0, g_enabled = 0, g_bad = 0;
static const char* g_text = 0; static CDeedReactionName* g_name = 0;
static CGameScriptInterface* g_self = 0;
CDeedReactionName::CDeedReactionName(int value, int length) { ++g_ctor; Value = value; g_text = (const char*)value; g_len = length; g_name = this; }
CDeedReactionName::~CDeedReactionName() { ++g_dtor; if (g_calls != 1 || this != g_name) ++g_bad; }
static void __fastcall SetDeedReactions(CGameScriptInterface* self, CDeedReactionName* a, int entity, CDeedReactionName* b, int enabled)
{
    ++g_calls;
    if (self != g_self || a != g_name || b != g_name || g_dtor != 0) ++g_bad;
    g_entity = entity; g_enabled = enabled;
}

int main()
{
    static SetDeedReactionsFn slots[0x300];
    slots[0x938 / 4] = SetDeedReactions;
    static CGameScriptInterface gsi; gsi.__vftable = slots; g_self = &gsi;
    CGameScriptInterface_EntitySetDeedReactionsEnabled(&gsi, 0, 111, 222);
    if (g_ctor == 1 && g_dtor == 1 && g_calls == 1 && g_bad == 0 && g_len == -1 && g_entity == 111 && g_enabled == 222
        && g_text != 0 && std::strcmp(g_text, "SG_DEED_REACTION_ROOT") == 0) {
        std::printf("CGameScriptInterface_0088f850_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL ctor=%d dtor=%d calls=%d bad=%d len=%d ent=%d en=%d\n", g_ctor, g_dtor, g_calls, g_bad, g_len, g_entity, g_enabled);
    return 1;
}
