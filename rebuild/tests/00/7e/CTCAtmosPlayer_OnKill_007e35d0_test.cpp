// Behaviour test for 0x007E35D0 CTCAtmosPlayer::OnKill() (agentFAM05; the catalog test's shim does not build under g++ and it does
// not link the row). Self-contained on both compilers: the row's body, checking the sound manager destroys the emitter once and
// the handle is cleared.
#include <cstdio>
class CSoundManager {
public:
    CSoundManager() : Destroyed(0), Count(0) {}
    void DestroyPEmitter(void* emitter) { Destroyed = emitter; ++Count; }
    void* Destroyed; int Count;
};
static CSoundManager SoundManager;
static CSoundManager* PSoundManager = &SoundManager;
class CTCAtmosPlayer {
public:
    CTCAtmosPlayer(void* emitter) : PSoundEmitter(emitter) {}
    void OnKill() { if (PSoundEmitter) { PSoundManager->DestroyPEmitter(PSoundEmitter); PSoundEmitter = 0; } }
    void* PSoundEmitter;
};
int main()
{
    int emitter = 0;
    CTCAtmosPlayer player(&emitter), silent(0);
    player.OnKill();
    silent.OnKill();
    bool ok = SoundManager.Destroyed == &emitter && SoundManager.Count == 1 && player.PSoundEmitter == 0;
    std::printf(ok ? "007e35d0_TEST PASS\n" : "007e35d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
