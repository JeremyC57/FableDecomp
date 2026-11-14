// Behaviour test for 0x00A3E520 CSoundManagerLH::StopMusic: with a sound system, the fade-out time is handed to the system's music
// player (the object of system slot 5) through its stop slot (2) -- not to the event manager and not to another player slot; without a
// sound system nothing is called.
#include <cstdio>
static int fails = 0, stops = 0, others = 0; static unsigned long seen = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
struct Player { virtual void v00() { ++others; } virtual void v01() { ++others; } virtual void Stop(unsigned long ms) { ++stops; seen = ms; } };
struct Events { virtual void v00() { ++others; } virtual void Set(unsigned long) { ++others; } };
struct System {
    Player player; Events events; int playerAsked, eventsAsked;
    virtual void v00() { ++others; } virtual void v01() { ++others; } virtual void v02() { ++others; } virtual void v03() { ++others; } virtual void v04() { ++others; }
    virtual Player* GetPlayer() { ++playerAsked; return &player; }
    virtual Events* GetEventManager() { ++eventsAsked; return &events; }
};
struct Manager { System* system; void StopMusic(unsigned long fadeOutMS) { if (system) system->GetPlayer()->Stop(fadeOutMS); } };
int main() {
    System s; s.playerAsked = 0; s.eventsAsked = 0;
    Manager m = { &s };
    m.StopMusic(1500);
    check(stops == 1 && seen == 1500, "fade-out reaches the player's stop slot");
    check(s.playerAsked == 1 && s.eventsAsked == 0 && others == 0, "only the player getter and the stop slot are used");
    m.StopMusic(0);
    check(stops == 2 && seen == 0, "a zero fade is forwarded too");
    Manager none = { 0 };
    none.StopMusic(7);
    check(stops == 2 && s.playerAsked == 2, "no sound system: nothing is called");
    if (!fails) std::printf("00a3e520_TEST PASS\n");
    return fails;
}
