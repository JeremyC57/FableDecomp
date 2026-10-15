// Behaviour test for 0x004afa60 (CQuestManager::OnActivateEntity): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Thing { char ScriptName[32]; unsigned char type; bool ActivatedScript; };
struct Script { const char* binds; int activations; };
static void OnActivate(Script** scripts, int n, Thing* t)
{
    if (std::strcmp(t->ScriptName, "NULL") != 0 && t->ScriptName[0] != 0 && t->type != 0x0d)
        for (int i = 0; i < n; ++i) if (scripts[i] && std::strcmp(scripts[i]->binds, t->ScriptName) == 0) ++scripts[i]->activations;
    t->ActivatedScript = true;
}

int main()
{
    Script s1 = { "Guard", 0 }, s2 = { "Trader", 0 }; Script* l[3] = { &s1, 0, &s2 };
    Thing g = { "Guard", 1, false }; OnActivate(l, 3, &g); CHECK(s1.activations == 1 && s2.activations == 0 && g.ActivatedScript);
    Thing n = { "NULL", 1, false }; OnActivate(l, 3, &n); CHECK(s1.activations == 1 && n.ActivatedScript);
    Thing x = { "Trader", 0x0d, false }; OnActivate(l, 3, &x); CHECK(s2.activations == 0 && x.ActivatedScript);
    if (g_fail) { std::printf("004afa60_TEST FAIL\n"); return 1; }
    std::printf("004afa60_TEST PASS\n");
    return 0;
}