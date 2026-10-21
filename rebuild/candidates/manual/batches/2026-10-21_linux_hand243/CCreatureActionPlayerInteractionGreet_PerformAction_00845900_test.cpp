#include <cstdio>
// Behaviour copy of CCreatureAction_PlayerInteractionGreet::PerformAction (0x00845900): two talked-to events and one greeting message when both things are alive.
struct Thing { bool alive; bool hasEvents; int events; const Thing* lastFrom; float x; };
struct Msg { long type; unsigned long flags; float x; const Thing* from; const Thing* to; long prio; int count; };
struct Action {
    Thing* creature; Thing* target; Msg* mgr;
    void PerformAction() {
        if (creature && creature->alive && target && target->alive) {
            if (target->hasEvents) { ++target->events; target->lastFrom = creature; }
            if (creature->hasEvents) { ++creature->events; creature->lastFrom = target; }
            mgr->type = 0x14; mgr->flags = 0x10; mgr->x = creature->x; mgr->from = creature; mgr->to = target; mgr->prio = 0; ++mgr->count;
        }
    }
};
int main()
{
    Thing c = { true, true, 0, 0, 2.5f }, t = { true, false, 0, 0, 1.0f }; Msg m = { 0, 0, 0, 0, 0, 9, 0 };
    Action a = { &c, &t, &m };
    a.PerformAction();
    bool ok = c.events == 1 && c.lastFrom == &t && t.events == 0 && m.count == 1 && m.type == 0x14 && m.flags == 0x10 && m.x == 2.5f && m.from == &c && m.to == &t && m.prio == 0;
    t.alive = false; a.PerformAction();
    ok = ok && m.count == 1 && c.events == 1;
    std::printf(ok ? "00845900_TEST PASS\n" : "00845900_TEST FAIL\n");
    return ok ? 0 : 1;
}
