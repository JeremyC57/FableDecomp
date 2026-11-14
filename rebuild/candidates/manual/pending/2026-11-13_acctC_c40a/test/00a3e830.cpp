// Behaviour test for 0x00A3E830 CSoundManagerLH::SetMonitorAllowedPostStopTailDurationMS: with a sound system, the duration is handed to
// the system's event manager (the object of system slot 6) through its slot 1 -- not to the music player; without a sound system
// nothing is called.
#include <cstdio>
static int fails = 0, sets = 0, others = 0; static unsigned long seen = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
struct Player { virtual void v00() { ++others; } virtual void v01() { ++others; } virtual void Stop(unsigned long) { ++others; } };
struct Events { virtual void v00() { ++others; } virtual void SetTail(unsigned long ms) { ++sets; seen = ms; } };
struct System {
    Player player; Events events; int playerAsked, eventsAsked;
    virtual void v00() { ++others; } virtual void v01() { ++others; } virtual void v02() { ++others; } virtual void v03() { ++others; } virtual void v04() { ++others; }
    virtual Player* GetPlayer() { ++playerAsked; return &player; }
    virtual Events* GetEventManager() { ++eventsAsked; return &events; }
};
struct Manager { System* system; void SetTail(unsigned long duration) { if (system) system->GetEventManager()->SetTail(duration); } };
int main() {
    System s; s.playerAsked = 0; s.eventsAsked = 0;
    Manager m = { &s };
    m.SetTail(250);
    check(sets == 1 && seen == 250, "duration reaches the event manager's slot 1");
    check(s.eventsAsked == 1 && s.playerAsked == 0 && others == 0, "only the event-manager getter and that slot are used");
    Manager none = { 0 };
    none.SetTail(9);
    check(sets == 1 && s.eventsAsked == 1 && seen == 250, "no sound system: nothing is called");
    if (!fails) std::printf("00a3e830_TEST PASS\n");
    return fails;
}
