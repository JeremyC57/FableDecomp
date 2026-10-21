// Behaviour test for CAIStateGroup_SleepOnFloor::Update (0x00969CB0): same control flow over mocks.
#include <cstdio>
#include <cstring>
struct CState {
    int moveResult; int animResult; int stopAfter; int idles; int terminated; bool Asleep;
    const char* anim; const char* modeAnim; const char* modeGroup; int modesAdded;
    int Move() { return moveResult; }
    int Perform(const char* a) { anim = a; return animResult; }
    bool IsStopping() { return idles >= stopAfter; }
    void DoNothing() { ++idles; }
    void Update()
    {
        bool reached = false;
        (void)reached;
        if (Move() == 2) {
            if (Perform("ST_SLEEP_ON_FLOOR_INTO") == 2) {
                modeAnim = "ST_SLEEP_ON_FLOOR_IDLE"; modeGroup = ""; ++modesAdded;
                Asleep = true;
                while (!IsStopping())
                    DoNothing();
            }
        }
        ++terminated;
    }
};
static void Reset(CState& s, int mv, int an)
{
    std::memset(&s, 0, sizeof(s)); s.moveResult = mv; s.animResult = an; s.stopAfter = 3;
}
int main()
{
    CState s;
    Reset(s, 2, 2); s.Update();
    bool ok = s.Asleep && s.modesAdded == 1 && s.idles == 3 && s.terminated == 1
        && std::strcmp(s.anim, "ST_SLEEP_ON_FLOOR_INTO") == 0 && std::strcmp(s.modeAnim, "ST_SLEEP_ON_FLOOR_IDLE") == 0
        && s.modeGroup[0] == 0;
    Reset(s, 2, 1); s.Update();
    ok = ok && !s.Asleep && s.modesAdded == 0 && s.idles == 0 && s.terminated == 1 && s.anim != 0;
    Reset(s, 1, 2); s.Update();
    ok = ok && !s.Asleep && s.anim == 0 && s.terminated == 1;
    if (!ok) { std::printf("00969cb0_TEST FAIL\n"); return 1; }
    std::printf("00969cb0_TEST PASS\n");
    return 0;
}
