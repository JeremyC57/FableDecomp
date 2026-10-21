#include <cstdio>
// Behaviour copy of CCreatureAction_UseObject::PerformAction (0x0069FE60): move-to at the creature, an item-used event on the thing, then the scripted hook or the plain use slot.
struct Thing { float x; bool hasEvents; bool hasHook; int events; const Thing* hookUser; const Thing* usedBy; };
struct Action {
    Thing* creature; Thing* used; float moveToX;
    void PerformAction() {
        moveToX = creature->x;
        if (used->hasEvents) ++used->events;
        if (used->hasHook) { used->hookUser = creature; return; }
        used->usedBy = creature;
    }
};
int main()
{
    Thing c = { 4.0f, false, false, 0, 0, 0 }, chest = { 0, true, false, 0, 0, 0 };
    Action a = { &c, &chest, 0 };
    a.PerformAction();
    bool ok = a.moveToX == 4.0f && chest.events == 1 && chest.hookUser == 0 && chest.usedBy == &c;
    Thing lever = { 0, false, true, 0, 0, 0 }; a.used = &lever; a.PerformAction();
    ok = ok && lever.events == 0 && lever.hookUser == &c && lever.usedBy == 0;
    std::printf(ok ? "0069fe60_TEST PASS\n" : "0069fe60_TEST FAIL\n");
    return ok ? 0 : 1;
}