#include <cstdio>

// Model of CTCHaste::FrameUpdate @ 0x00800000.
static long g_removed = -1;
struct CThing { void RemoveTC(long type) { g_removed = type; } };
struct CTCHaste {
    CThing* ParentThing; float Multiplier; bool ToTerminate; long PauseFrames;
    void FrameUpdate()
    {
        if (PauseFrames >= 0) --PauseFrames;
        if (ToTerminate) ParentThing->RemoveTC(0x8c);
    }
};

int main()
{
    CThing t; CTCHaste h; h.ParentThing = &t; h.Multiplier = 1.0f; h.ToTerminate = false; h.PauseFrames = 2;
    h.FrameUpdate();
    if (h.PauseFrames != 1 || g_removed != -1) { std::printf("00800000_TEST FAIL\n"); return 1; }
    h.PauseFrames = -1; h.FrameUpdate();
    if (h.PauseFrames != -1) { std::printf("00800000_TEST FAIL\n"); return 1; }
    h.ToTerminate = true; h.FrameUpdate();
    if (g_removed != 0x8c) { std::printf("00800000_TEST FAIL\n"); return 1; }
    std::printf("00800000_TEST PASS\n");
    return 0;
}
