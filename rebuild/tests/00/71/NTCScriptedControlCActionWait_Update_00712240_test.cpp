// Behaviour test for 0x00712240 NTCScriptedControl::CActionWait::Update
#include <cstdio>
struct CActionWait {
    bool Terminated;
    long NoFramesToWait;
    long UpdateCounter;
    void Update()
    {
        ++UpdateCounter;
        if (NoFramesToWait > 0 && UpdateCounter > NoFramesToWait)
            Terminated = true;
    }
};
int main()
{
    CActionWait w; w.Terminated = false; w.NoFramesToWait = 3; w.UpdateCounter = 0;
    w.Update(); w.Update(); w.Update();
    int ok = !w.Terminated && w.UpdateCounter == 3;
    w.Update();
    ok = ok && w.Terminated && w.UpdateCounter == 4;
    CActionWait f; f.Terminated = false; f.NoFramesToWait = 0; f.UpdateCounter = 100;
    f.Update();
    ok = ok && !f.Terminated && f.UpdateCounter == 101;
    if (!ok) { std::printf("00712240_TEST FAIL\n"); return 1; }
    std::printf("00712240_TEST PASS\n");
    return 0;
}