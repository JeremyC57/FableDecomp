// Behaviour test for 00c15cd0: SetPlaybackValue -- under the module lock, forwards the unsigned value to the player as a float (exact for values above 2^31)
#include <cstdio>

static int g_lockDepth = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static unsigned char g_section[0x18];

struct CSoundPlayer {
    float m_value; int m_lockedCalls;
    void SetValue(float value) { m_value = value; if (g_lockDepth == 1) ++m_lockedCalls; }
};

struct CSoundInstance {
    CSoundPlayer* m_pPlayer;
    void SetPlaybackValue(unsigned int value)
    {
        CScopedLock lock(g_section);
        m_pPlayer->SetValue((float)value);
    }
};

int main()
{
    CSoundPlayer player = { 0.0f, 0 };
    CSoundInstance instance = { &player };
    instance.SetPlaybackValue(1500);
    if (player.m_value != 1500.0f || player.m_lockedCalls != 1 || g_lockDepth != 0) { std::printf("00c15cd0_TEST FAIL\n"); return 1; }
    instance.SetPlaybackValue(0xC0000000u);    // unsigned: positive 3221225472, not a negative int
    if (player.m_value != 3221225472.0f) { std::printf("00c15cd0_TEST FAIL\n"); return 1; }
    std::printf("00c15cd0_TEST PASS\n");
    return 0;
}