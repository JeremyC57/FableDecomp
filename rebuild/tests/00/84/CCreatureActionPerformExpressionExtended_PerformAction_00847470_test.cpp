// behaviour test for 0x00847470 CCreatureAction_PerformExpressionExtended::PerformAction(): when the perform-to thing is set
// and has a CTCEntityEvents, post a new "expression performed to" event (performer, frames running + FrameCount, the
// expression def, -1, -1); otherwise nothing
#include <cstdio>
struct Def { int id; };
struct Event {
    void* performer; long frame; const Def* def; long a, b;
    Event(void* p, long f, const Def* d, long x, long y) : performer(p), frame(f), def(d), a(x), b(y) {}
};
struct Events {
    Event* last; int count;
    Events() : last(0), count(0) {}
    void AddEvent(Event* e) { delete last; last = e; ++count; }
    ~Events() { delete last; }
};
struct Thing { Events* events; bool GetTC(Events** out) { if (!events) return false; *out = events; return true; } };
struct Action {
    void* PCreature; Thing* PPerformToThing; const Def* PExpressionDef; long FramesRunning; long FrameCount;
    long GetFramesRunning() const { return FramesRunning; }
    void PerformAction()
    {
        Events* events;
        if (PPerformToThing && PPerformToThing->GetTC(&events))
            events->AddEvent(new Event(PCreature, GetFramesRunning() + FrameCount, PExpressionDef, -1, -1));
    }
};
int main()
{
    int fails = 0, creature = 0;
    Def def = { 7 };
    Events ev;
    Thing target = { &ev };
    Action a = { &creature, &target, &def, 10, 5 };
    a.PerformAction();
    if (ev.count != 1 || !ev.last || ev.last->performer != &creature || ev.last->frame != 15 || ev.last->def != &def
        || ev.last->a != -1 || ev.last->b != -1) fails |= 1;
    Thing noEvents = { 0 };
    a.PPerformToThing = &noEvents; a.PerformAction();
    a.PPerformToThing = 0; a.PerformAction();
    if (ev.count != 1) fails |= 2;
    if (fails) { std::printf("00847470_TEST FAIL %d\n", fails); return 1; }
    std::printf("00847470_TEST PASS\n");
    return 0;
}