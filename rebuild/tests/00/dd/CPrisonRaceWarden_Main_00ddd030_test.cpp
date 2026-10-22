// Behaviour test for CPrisonRaceWarden::Main @ 0x00DDD030: script the warden, wait while the race is flagged ended or the
// hero is 5+ units away; then mark the hero's win, play CS_PRISON_RACE_END (first win) or _REPEAT, pause/unpause the world,
// mark RaceEnded and idle until termination.
#include <cstdio>
#include <string>
#include <vector>
struct Warden {
    bool RaceEnded, HeroWonRace, EverWon, Paused; const float* Dist; int At, Frames, Terminate; std::vector<std::string> Log;
    bool Term() const { return Frames >= Terminate; }
    void Frame() { ++Frames; ++At; }
    void Main()
    {
        Log.push_back("script me");
        while (RaceEnded || !(Dist[At] < 5.0f)) { Frame(); if (Term()) { Log.push_back("release"); return; } }
        bool firstTime = !EverWon;
        HeroWonRace = true;
        Paused = true; Log.push_back(firstTime ? "CS_PRISON_RACE_END" : "CS_PRISON_RACE_END_REPEAT"); Paused = false;
        RaceEnded = true;
        do Frame(); while (!Term());
        Log.push_back("release");
    }
};
int main()
{
    float d1[] = { 20.0f, 9.0f, 4.0f, 4.0f, 4.0f, 4.0f };
    Warden a = { false, false, false, false, d1, 0, 0, 5, std::vector<std::string>() };
    a.Main();
    float d2[] = { 1.0f, 1.0f };
    Warden b = { false, false, true, false, d2, 0, 0, 1, std::vector<std::string>() };
    b.Main();
    float d3[] = { 1.0f, 1.0f, 1.0f };
    Warden c = { true, false, false, false, d3, 0, 0, 2, std::vector<std::string>() };
    c.Main();
    bool ok = a.HeroWonRace && a.RaceEnded && a.Log[1] == "CS_PRISON_RACE_END" && !a.Paused && a.Frames == 5 &&
              b.Log[1] == "CS_PRISON_RACE_END_REPEAT" && b.Frames == 1 &&
              !c.HeroWonRace && c.Log.size() == 2 && c.Log[1] == "release";
    if (!ok) { std::printf("00ddd030_TEST FAIL\n"); return 1; }
    std::printf("00ddd030_TEST PASS\n");
    return 0;
}