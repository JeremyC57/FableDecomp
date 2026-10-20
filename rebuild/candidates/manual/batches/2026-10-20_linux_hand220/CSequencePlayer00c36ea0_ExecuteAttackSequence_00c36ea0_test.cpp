#include <cstdio>
struct C3DVector { float x, y, z; };
struct Track { float Start, End; int plays; float lastZ, lastDur; int finished; void Play(const C3DVector& p, float d) { ++plays; lastZ = p.z; lastDur = d; } void Finish(unsigned long) { ++finished; } };
struct Player {
    Track tracks[3]; unsigned long count; int restarts, begins, ends; float trackTime;
    void Restart() { ++restarts; } void Begin() { ++begins; } void End() { ++ends; }
    Track* FindTrack(unsigned long i) { return (i == 1) ? 0 : &tracks[i]; }
    void ExecuteAttackSequence(long restart)
    {
        if (restart) Restart();
        Begin();
        for (unsigned long i = 0; i < count; ++i) {
            Track* t = FindTrack(i);
            if (t) {
                C3DVector pos = { 1, 2, 0 };
                pos.z = trackTime + t->Start;
                t->Play(pos, t->End - t->Start);
                if (restart && i == count - 1) t->Finish(i);
            }
        }
        End();
    }
};
int main()
{
    Player p = Player();
    p.count = 3; p.trackTime = 2.0f;
    for (int i = 0; i < 3; ++i) { p.tracks[i].Start = 1.0f * i; p.tracks[i].End = 5.0f; p.tracks[i].plays = 0; p.tracks[i].finished = 0; }
    p.restarts = p.begins = p.ends = 0;
    p.ExecuteAttackSequence(1);
    if (p.restarts != 1 || p.begins != 1 || p.ends != 1 || p.tracks[0].plays != 1 || p.tracks[1].plays != 0 || p.tracks[2].lastZ != 4.0f || p.tracks[2].lastDur != 3.0f || p.tracks[2].finished != 1) { std::printf("00c36ea0_TEST FAIL\n"); return 1; }
    p.ExecuteAttackSequence(0);
    if (p.restarts != 1 || p.tracks[2].finished != 1 || p.tracks[0].plays != 2) { std::printf("00c36ea0_TEST FAIL\n"); return 1; }
    std::printf("00c36ea0_TEST PASS\n");
    return 0;
}
