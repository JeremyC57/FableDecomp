#include <cstdio>
#include <cstdlib>
#include <vector>
// Behaviour model of RunVillagerComments: the script interface is simulated (frames, timer, hero distances).
struct Sim {
    int frames, timerLeft, terminateAt, succeedAfterPicks, picks;
    std::vector<float> dist2;      // squared hero distance per villager (changes per frame)
    std::vector<unsigned char> speak;
    bool missionSucceeded;
    long registered, deregistered;
    void NewScriptFrame() { ++frames; if (timerLeft > 0) --timerLeft; if (frames == 40) dist2[2] = 9.0f; }
    bool IsActiveThreadTerminating() { return terminateAt >= 0 && frames >= terminateAt; }
};
struct TRetVoid { void Return() { } };
static void RunVillagerComments(Sim& s)
{
    TRetVoid _Fail_Val;
    ++s.registered;
    struct Dereg { Sim& s; ~Dereg() { ++s.deregistered; } } timer = { s };
    long n = (long)s.dist2.size();
    std::vector<long> candidates(n);
    while (!s.missionSucceeded) {
        s.NewScriptFrame();
        if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
        s.timerLeft = 25;
        while (s.timerLeft != 0) {
            s.NewScriptFrame();
            if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
        long count;
        do {
            s.NewScriptFrame();
            if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
            count = 0;
            for (long i = 0; i < n; ++i) {
                if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (s.dist2[i] <= 25.0f) {
                    if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
                    candidates[count] = i;
                    count++;
                }
            }
            if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
        } while (count <= 0);
        if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
        s.speak[candidates[std::rand() % count]] = 1;
        if (++s.picks >= s.succeedAfterPicks) s.missionSucceeded = true;
        if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (s.IsActiveThreadTerminating()) return _Fail_Val.Return();
}
int main()
{
    Sim s = { 0, 0, -1, 1, 0, std::vector<float>(4, 100.0f), std::vector<unsigned char>(4, 0), false, 0, 0 };
    s.dist2[2] = 100.0f;
    RunVillagerComments(s);
    bool ok = s.speak[2] == 1 && s.speak[0] == 0 && s.speak[1] == 0 && s.speak[3] == 0 && s.frames == 40
           && s.registered == 1 && s.deregistered == 1;
    Sim t = { 0, 0, 10, 1, 0, std::vector<float>(3, 1.0f), std::vector<unsigned char>(3, 0), false, 0, 0 };
    RunVillagerComments(t);   // terminated during the 25-frame wait: nobody flagged, timer still deregistered
    ok = ok && t.frames == 10 && t.speak[0] + t.speak[1] + t.speak[2] == 0 && t.deregistered == 1;
    Sim u = { 0, 0, -1, 1, 0, std::vector<float>(2, 1.0f), std::vector<unsigned char>(2, 0), true, 0, 0 };
    RunVillagerComments(u);   // mission already succeeded: nothing runs
    ok = ok && u.frames == 0 && u.deregistered == 1;
    if (!ok) { std::printf("00d38d20_TEST FAIL\n"); return 1; }
    std::printf("00d38d20_TEST PASS\n");
    return 0;
}