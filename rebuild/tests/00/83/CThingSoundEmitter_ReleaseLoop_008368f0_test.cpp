// Behaviour test for CThingSoundEmitter::ReleaseLoop (0x008368F0): links the reconstructed object; the sound object is a
// recording stub, the game-state global is defined here.
#include <cstdio>
#include <cstring>
void* g_pGameState = 0;
static int g_calls = 0; static unsigned long g_id = 0;
struct CSoundPlayer {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {}
    virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {}
    virtual void StopSpecificSample(unsigned long) { g_calls += 100; }
    virtual void v010() {} virtual void v011() {} virtual void v012() {}
    virtual void ReleaseLoop(unsigned long sampleID) { ++g_calls; g_id = sampleID; }
};
class CThingSoundEmitter {
public:
    unsigned char _pad00[0x91]; unsigned char Dead : 1; unsigned char _flags91_1 : 7; unsigned char _pad92[0x6];
    CSoundPlayer* m_pSoundPlayer;
    void ReleaseLoop(unsigned long sampleID);
};
int main()
{
    CSoundPlayer player; CThingSoundEmitter e; std::memset(&e, 0, sizeof e); e.m_pSoundPlayer = &player;
    e.ReleaseLoop(7);                            // no game state: nothing
    if (g_calls != 0) { std::printf("008368f0_TEST FAIL 1\n"); return 1; }
    int gs; g_pGameState = &gs;
    e.Dead = 1; e.ReleaseLoop(7);                // dead: nothing
    if (g_calls != 0) { std::printf("008368f0_TEST FAIL 2\n"); return 1; }
    e.Dead = 0; e.ReleaseLoop(42);
    if (g_calls != 1 || g_id != 42) { std::printf("008368f0_TEST FAIL 3\n"); return 1; }
    std::printf("008368f0_TEST PASS\n");
    return 0;
}
