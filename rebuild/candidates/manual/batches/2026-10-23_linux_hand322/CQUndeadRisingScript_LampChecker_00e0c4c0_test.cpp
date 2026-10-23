#include <cstdio>
// behaviour test: CQ_UndeadRisingScript::LampChecker -- after each graveyard reload, relight activated lamps; retry the
// failed ones once a second until all are lit
static int g_frame = 0, g_creates = 0, g_pauses = 0;
static int g_failuresLeft[8];
struct Script {
    bool LightsActivated[8]; int loaded; int relightRounds;
    bool IsActiveThreadTerminating() { return g_frame > 40; }
    bool IsLevelLoaded() { return loaded > 0; }
    void Frame() { ++g_frame; if (g_frame == 1) loaded = 1; if (g_frame == 3) loaded = 0; if (g_frame == 5) loaded = 1; }
    bool CreateEffectAlive(int i) { ++g_creates; if (g_failuresLeft[i] > 0) { --g_failuresLeft[i]; return false; } return true; }
    void Run()
    {
        bool lampOut[8];
        while (!IsLevelLoaded()) { Frame(); if (IsActiveThreadTerminating()) return; }
        for (long i = 0; i < 8; ++i) lampOut[i] = false;
        while (!IsActiveThreadTerminating()) {
            while (IsLevelLoaded()) { Frame(); if (IsActiveThreadTerminating()) return; }
            while (!IsLevelLoaded()) { Frame(); if (IsActiveThreadTerminating()) return; }
            bool anyOut = false;
            for (long i = 0; i < 8; ++i) if (LightsActivated[i] && !CreateEffectAlive(i)) { lampOut[i] = true; anyOut = true; }
            while (anyOut && IsLevelLoaded()) {
                Frame(); if (IsActiveThreadTerminating()) return;
                ++g_pauses; ++relightRounds;
                anyOut = false;
                for (long i = 0; i < 8; ++i) if (lampOut[i]) { if (!CreateEffectAlive(i)) anyOut = true; else lampOut[i] = false; }
            }
            if (IsActiveThreadTerminating()) return;
            loaded = 0; Frame();
        }
    }
};
int main()
{
    Script s; s.loaded = 0; s.relightRounds = 0;
    for (int i = 0; i < 8; ++i) { s.LightsActivated[i] = (i % 2) == 0; g_failuresLeft[i] = 0; }
    g_failuresLeft[2] = 2;
    s.Run();
    if (s.relightRounds != 2 || g_pauses != 2 || g_creates < 4 + 2) { std::printf("00e0c4c0_TEST FAIL\n"); return 1; }
    std::printf("00e0c4c0_TEST PASS\n");
    return 0;
}
