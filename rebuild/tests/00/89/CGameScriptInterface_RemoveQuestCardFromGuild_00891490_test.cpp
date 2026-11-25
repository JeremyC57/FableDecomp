// Behaviour test for 0x00891490 CGameScriptInterface::RemoveQuestCardFromGuild(int questId) (NF415: linked): links the row object (the
// function is declared here, not defined) and defines its callees under the row's spellings. Checks: the quest manager maps the id,
// a temporary string is built from the result and checked against retail's pooled "" (0x0122D70E); only a passing check removes the
// card; the temporary is destroyed either way.
#include <cstdio>
#include <cstring>

struct Str {
    void* p;
    void ctor(int v);
    char check(int c);
    void dtor();
};
struct Guild {
    void* vt;
    char pad[0x100];
    int  map(int id);
    void remove(Str* s);
};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
void __stdcall CGameScriptInterface_RemoveQuestCardFromGuild(int questId);   // the row (linked)

static Guild g_guild;
static CCountedPointer<CQuestManager> MakeManagerPointer() { CCountedPointer<CQuestManager> p; p.Data = (CQuestManager*)&g_guild; return p; }
template <> CCountedPointer<CQuestManager> CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass = MakeManagerPointer();   // the quest manager (the row reads it as the guild)
static int g_mapArg = -1, g_ctorVal = 0, g_checks = 0, g_bad = 0;
static bool g_removed = false, g_dtored = false, g_returnCheck = true;
static Str* g_tmp = 0;
int Guild::map(int id) { if (this != &g_guild) ++g_bad; g_mapArg = id; return id + 100; }
void Str::ctor(int v) { g_ctorVal = v; g_tmp = this; }
char Str::check(int c) { ++g_checks; const char* s = (const char*)c; if (this != g_tmp || s == 0 || std::strcmp(s, "") != 0) ++g_bad; return g_returnCheck ? 1 : 0; }
void Guild::remove(Str* s) { if (this != &g_guild || s != g_tmp) ++g_bad; g_removed = true; }
void Str::dtor() { if (this != g_tmp) ++g_bad; g_dtored = true; }

int main()
{
    g_returnCheck = true;
    CGameScriptInterface_RemoveQuestCardFromGuild(42);
    if (g_mapArg != 42 || g_ctorVal != 142 || g_checks != 1 || !g_removed || !g_dtored) { std::printf("FAIL pass path\n"); return 1; }
    g_removed = false; g_dtored = false; g_returnCheck = false;
    CGameScriptInterface_RemoveQuestCardFromGuild(7);
    if (g_removed || !g_dtored || g_checks != 2) { std::printf("FAIL false path\n"); return 1; }
    if (g_bad) { std::printf("FAIL argument check (%d)\n", g_bad); return 1; }
    std::printf("CGameScriptInterface_00891490_TEST PASS\n");
    return 0;
}
