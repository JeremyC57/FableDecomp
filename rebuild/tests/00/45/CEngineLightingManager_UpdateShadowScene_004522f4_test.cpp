// Behaviour test for 0x004522F4 CEngineLightingManager::UpdateShadowScene (NF413): links the row object (the function is declared
// here, not defined) and checks the scope marker text (retail's pooled "") and the mode routing (2: holder, 3: sink, other: none).
#include <cstdio>

// NF413: the row spells retail's literal now, so compare the text it passes, not an address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

struct CShadowArg { int tag; };

static int g_consumed = 0;
static CShadowArg* g_consumedArg = 0;
struct CShadowSink {
    void Consume(CShadowArg* a);   // the row's callee, defined here
};
void CShadowSink::Consume(CShadowArg* a) { g_consumed++; g_consumedArg = a; }

class CEngineLightingManager { public:
    char pad0[0x18];
    int  mode;      // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;    // +0x24
    void* obj28;    // +0x28
};

static int g_handled = 0;
static void* g_seenArg = 0;
static void* g_seenObj = 0;
struct CLocalHolder {
    int v;
    void Handle(void* arg, void* obj);   // the row's callee, defined here
};
void CLocalHolder::Handle(void* arg, void* obj) { g_handled++; g_seenArg = arg; g_seenObj = obj; }

static int g_scopeCalls = 0;
static const void* g_scopeTag = 0;
extern "C" void __stdcall EnterShadowScope(void* tag) { g_scopeTag = tag; g_scopeCalls++; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, CShadowArg** ppArg);   // linked from the row object

static void reset() { g_scopeCalls = 0; g_scopeTag = 0; g_handled = 0; g_seenArg = 0; g_seenObj = 0; g_consumed = 0; g_consumedArg = 0; }

int main() {
    CShadowArg arg; arg.tag = 7;
    CShadowArg* pArg = &arg;

    // mode == 2 : local.Handle(obj24, ppArg)
    CEngineLightingManager m2; m2.mode = 2;
    void* fake24 = (void*)0x1234;
    m2.obj24 = fake24; m2.obj28 = 0;
    reset();
    CEngineLightingManager_UpdateShadowScene(&m2, 0, &pArg);
    if (g_scopeCalls != 1 || !RetailTextEquals(g_scopeTag, "")) { std::printf("FAIL scope m2\n"); return 1; }
    if (g_handled != 1 || g_seenArg != fake24) { std::printf("FAIL m2 arg\n"); return 1; }
    if (g_seenObj != (void*)&pArg) { std::printf("FAIL m2 obj\n"); return 1; }
    if (g_consumed != 0) { std::printf("FAIL m2 consumed\n"); return 1; }

    // mode == 3 : sink->Consume(*ppArg)
    CShadowSink sink;
    CEngineLightingManager m3; m3.mode = 3;
    m3.obj24 = 0; m3.obj28 = &sink;
    reset();
    CEngineLightingManager_UpdateShadowScene(&m3, 0, &pArg);
    if (g_scopeCalls != 1 || !RetailTextEquals(g_scopeTag, "")) { std::printf("FAIL scope m3\n"); return 1; }
    if (g_consumed != 1 || g_consumedArg != pArg || g_handled != 0) { std::printf("FAIL m3\n"); return 1; }

    // other mode : only scope enter, no routing
    CEngineLightingManager m0; m0.mode = 0;
    m0.obj24 = 0; m0.obj28 = 0;
    reset();
    CEngineLightingManager_UpdateShadowScene(&m0, 0, &pArg);
    if (g_scopeCalls != 1 || !RetailTextEquals(g_scopeTag, "")) { std::printf("FAIL scope m0\n"); return 1; }
    if (g_consumed != 0 || g_handled != 0) { std::printf("FAIL m0 routed\n"); return 1; }

    std::printf("CEngineLightingManager_004522f4_TEST PASS\n");
    return 0;
}
