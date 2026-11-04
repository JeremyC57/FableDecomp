// Behaviour test for CProphetCage::Main @ 0x00D38F70: wait until this cage's prophet is released or killed, then for the
// OPENCAGE flag; burst with CRYSTAL_IMPLODE_01 (released) or CRYSTAL_OPEN_01 (killed), fade out, clear the flag, activate.
#include <cstdio>
#include <string>
#include <vector>
struct Cage {
    bool Released[3], Killed[3], OpenCage, IWasActivated; int MyID, Frames, FlagAt, ReleaseAt; std::vector<std::string> Log;
    void Frame() { ++Frames; if (Frames == ReleaseAt) Released[MyID] = true; if (Frames == FlagAt) OpenCage = true; }
    void Burst(const char* fx) {
        while (OpenCage != true) Frame();
        Log.push_back(fx); Log.push_back("fade"); OpenCage = false; IWasActivated = true;
    }
    void Main(int idleFrames)
    {
        Frame();
        while (!IWasActivated) {
            Frame();
            if (Released[MyID]) Burst("CRYSTAL_IMPLODE_01");
            else if (Killed[MyID]) Burst("CRYSTAL_OPEN_01");
        }
        for (int i = 0; i < idleFrames; ++i) Frame();
    }
};
int main()
{
    Cage a = { { false, false, false }, { false, false, false }, false, false, 1, 0, 6, 3, std::vector<std::string>() };
    a.Main(0);
    Cage b = { { false, false, false }, { false, false, true }, true, false, 2, 0, 0, 0, std::vector<std::string>() };
    b.Main(2);
    bool ok = a.Log.size() == 2 && a.Log[0] == "CRYSTAL_IMPLODE_01" && a.IWasActivated && !a.OpenCage && a.Frames == 6 &&
              b.Log.size() == 2 && b.Log[0] == "CRYSTAL_OPEN_01" && !b.OpenCage && b.Frames == 4;
    if (!ok) { std::printf("00d38f70_TEST FAIL\n"); return 1; }
    std::printf("00d38f70_TEST PASS\n");
    return 0;
}