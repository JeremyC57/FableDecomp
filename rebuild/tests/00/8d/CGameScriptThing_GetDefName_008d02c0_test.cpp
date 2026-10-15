#include <cstdio>
#include <cstring>
// Host model of CGameScriptThing::GetDefName: the def name of a live thing through the def-string table, else "".
struct DefString { long index; };
struct Thing { bool dead; long def; DefString GetDefName() const { DefString d; d.index = def; return d; } };
static const char* g_table[3] = { "OBJECT_A", "CREATURE_B", "HERO" };
struct Str { const char* text; Str(const char* t) : text(t) {} };
static Str GetString(DefString d) { return Str(g_table[d.index]); }
struct GameScriptThing {
    Thing* item;
    Thing* GetPItem() const { return item; }
    Str GetDefName() const
    {
        if (GetPItem() && !GetPItem()->dead) {
            Str name = GetString(GetPItem()->GetDefName());
            return name;
        }
        return Str("");
    }
};
int main()
{
    int fail = 0;
    Thing t; t.dead = false; t.def = 1;
    GameScriptThing g; g.item = 0;
    if (std::strcmp(g.GetDefName().text, "") != 0) fail = 1;
    g.item = &t;
    if (std::strcmp(g.GetDefName().text, "CREATURE_B") != 0) fail = 1;
    t.dead = true;
    if (std::strcmp(g.GetDefName().text, "") != 0) fail = 1;
    if (fail) { std::printf("008d02c0_TEST FAIL\n"); return 1; }
    std::printf("008d02c0_TEST PASS\n");
    return 0;
}