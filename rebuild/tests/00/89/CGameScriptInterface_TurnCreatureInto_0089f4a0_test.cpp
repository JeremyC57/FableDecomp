// Behaviour test for CGameScriptInterface::TurnCreatureInto (0x0089F4A0): dead things give the null result, an unchanged def
// returns the thing itself, an AI creature is re-created with its script name and persistence, the player creature is
// re-created persisted with copies of the interfaces it has and becomes the main player's creature; the old thing dies.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Thing {
    int id; bool alive; int type; int defIndex; bool persisted; bool killed;
    unsigned int present[9];
    char copied[64];
};
static Thing g_new;
static bool g_defOk;

static bool HasTC(const Thing* t, int type) { return (t->present[type >> 5] & (1u << (type & 31))) != 0; }
static void CopyTC(Thing* to, int type, const char* how)
{
    char b[16]; std::sprintf(b, "%s%x,", how, type); std::strcat(to->copied, b);
}
static Thing* Create(const Thing* old, int index, int type)
{
    std::memset(&g_new, 0, sizeof g_new);
    g_new.id = 99; g_new.alive = true; g_new.type = type; g_new.defIndex = index; (void)old;
    return &g_new;
}
static int TurnCreatureInto(Thing* thing, int index)
{
    int result = 0;
    if (thing->alive) {
        if (thing->defIndex == index) {
            result = thing->id;
        } else {
            if (thing->type == 1) {
                if (g_defOk) {
                    Thing* c = Create(thing, index, 1);
                    c->persisted = thing->persisted;
                    result = c->id;
                }
            } else if (thing->type == 2) {
                if (g_defOk) {
                    Log("deregister");
                    Thing* c = Create(thing, index, 2);
                    c->persisted = true;
                    result = c->id;
                    static const int add[5] = { 0x29, 0x04, 0x68, 0x1b, 0xfc };
                    static const int ifx[3] = { 0x11, 0x13, 0x12 };
                    for (int i = 0; i < 5; ++i) if (HasTC(thing, add[i])) CopyTC(c, add[i], "a");
                    for (int i = 0; i < 3; ++i) if (HasTC(thing, ifx[i])) CopyTC(c, ifx[i], "c");
                    Log("main player");
                }
            }
            thing->killed = true;
        }
    }
    return result;
}
int main()
{
    Thing t; std::memset(&t, 0, sizeof t);
    t.id = 7; t.alive = false; t.type = 1; t.defIndex = 3;
    g_defOk = true;
    bool ok = TurnCreatureInto(&t, 4) == 0 && !t.killed;
    t.alive = true;
    ok = ok && TurnCreatureInto(&t, 3) == 7 && !t.killed;
    t.persisted = true;
    ok = ok && TurnCreatureInto(&t, 4) == 99 && t.killed && g_new.persisted && g_new.type == 1;
    t.killed = false; t.type = 2; t.persisted = false;
    t.present[0x29 >> 5] |= 1u << (0x29 & 31);
    t.present[0x12 >> 5] |= 1u << (0x12 & 31);
    t.present[0xfc >> 5] |= 1u << (0xfc & 31);
    g_log[0] = 0;
    ok = ok && TurnCreatureInto(&t, 5) == 99 && t.killed && g_new.persisted && std::strcmp(g_new.copied, "a29,afc,c12,") == 0
         && std::strcmp(g_log, "deregister;main player;") == 0;
    t.killed = false; g_defOk = false;
    ok = ok && TurnCreatureInto(&t, 5) == 0 && t.killed;
    t.killed = false; t.type = 3; g_defOk = true;
    ok = ok && TurnCreatureInto(&t, 5) == 0 && t.killed;
    if (!ok) {
        std::printf("0089f4a0_TEST FAIL\n");
        return 1;
    }
    std::printf("0089f4a0_TEST PASS\n");
    return 0;
}