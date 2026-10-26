// Behaviour test for CThingSoundEmitter::StopSpecificSample (0x008368D0): forwards only with a game state and a live thing.
#include <cstdio>
void* g_pGameState = 0;
static unsigned int g_stopped = 0;
static int g_calls = 0;

struct CSoundPlayer {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {}
    virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {}
    virtual void StopSpecificSample(unsigned int sampleID) { g_stopped = sampleID; ++g_calls; }
};

class CThingSoundEmitter { public:
    unsigned char  Dead;
    CSoundPlayer*  m_pSoundPlayer;
    bool IsActive() const { return g_pGameState && !Dead; }
    void StopSpecificSample(unsigned int sampleID)
    {
        if (IsActive())
            m_pSoundPlayer->StopSpecificSample(sampleID);
    }
};

int main()
{
    CSoundPlayer player;
    CThingSoundEmitter e;
    e.Dead = 0;
    e.m_pSoundPlayer = &player;
    int state = 1;
    e.StopSpecificSample(7);                       // no game state: nothing
    if (g_calls != 0) { std::printf("008368d0_TEST FAIL\n"); return 1; }
    g_pGameState = &state;
    e.StopSpecificSample(9);
    if (g_calls != 1 || g_stopped != 9) { std::printf("008368d0_TEST FAIL\n"); return 1; }
    e.Dead = 1;                                    // dead thing: nothing
    e.StopSpecificSample(11);
    if (g_calls != 1 || g_stopped != 9) { std::printf("008368d0_TEST FAIL\n"); return 1; }
    std::printf("008368d0_TEST PASS\n");
    return 0;
}