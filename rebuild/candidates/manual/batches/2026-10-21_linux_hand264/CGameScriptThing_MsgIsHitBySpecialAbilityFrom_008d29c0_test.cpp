// Behaviour test for CGameScriptThing::MsgIsHitBySpecialAbilityFrom @ 0x008D29C0: an ability event (type 27) inside
// (last, current] for the requested ability matches an empty name, or a live performer with the given script name.
#include <cstdio>
#include <cstring>
struct Thing { bool dead; const char* scriptName; };
struct Event { int type; int frame; int ability; Thing* performer; };
static bool IsThingAlive(Thing* const& t) { return t && !t->dead; }
static bool Query(bool selfAlive, const Event* list, int count, int last, int current, int ability, const char* name)
{
    if (!selfAlive) return false;
    for (int i = 0; i < count; ++i) {
        const Event& e = list[i];
        if (e.type == 27 && e.frame > last && e.frame <= current) {
            bool hit = e.ability == ability && ((name == 0 || std::strcmp(name, "") == 0)
                || (IsThingAlive(e.performer) && std::strcmp(e.performer->scriptName, name) == 0));
            if (hit) return true;
        }
    }
    return false;
}
int main()
{
    Thing hero = { false, "HERO" }; Thing ghost = { true, "GHOST" };
    Event list[3] = { { 27, 5, 3, &hero }, { 27, 6, 4, &ghost }, { 26, 7, 4, &hero } };
    bool ok = Query(true, list, 3, 4, 10, 3, "HERO") && Query(true, list, 3, 4, 10, 3, "") && Query(true, list, 3, 4, 10, 4, "");
    ok = ok && !Query(true, list, 3, 4, 10, 4, "GHOST") && !Query(true, list, 3, 4, 10, 4, "HERO") && !Query(true, list, 3, 5, 10, 3, "HERO");
    ok = ok && !Query(false, list, 3, 4, 10, 3, "HERO") && !Query(true, list, 3, 4, 10, 3, "BANDIT");
    if (!ok) { std::printf("008d29c0_TEST FAIL\n"); return 1; }
    std::printf("008d29c0_TEST PASS\n");
    return 0;
}
