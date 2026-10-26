// Behaviour test for 0x0066AE60 CTCSoundPlayer::IsAttachedSoundLoadedAndPlaying: nothing without the sound system, without an attached
// emitter or with a dead emitter; otherwise the emitter's IsSampleLoadedAndPlaying for the sample id.
#include <cstdio>
#define FAIL do { std::printf("0066ae60_TEST FAIL\n"); return 1; } while (0)
void* g_pSoundSystem = 0;
class CThingSoundEmitter {
public:
    unsigned char Dead;
    unsigned long Playing, Last;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const { return sampleID == Playing; }
};
class CIntelligentPointer {
public:
    CThingSoundEmitter* PItem;
    mutable int Fetches;
    CThingSoundEmitter* GetPItem() const { ++Fetches; return PItem; }
};
class CTCSoundPlayer {
public:
    CIntelligentPointer PAttachedEmitter;
    bool IsAttachedSoundLoadedAndPlaying(unsigned long sampleID) const;
};
bool CTCSoundPlayer::IsAttachedSoundLoadedAndPlaying(unsigned long sampleID) const
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!PAttachedEmitter.GetPItem()->Dead)
                return PAttachedEmitter.GetPItem()->IsSampleLoadedAndPlaying(sampleID);
        }
    }
    return false;
}
int main()
{
    int sys = 1;
    CThingSoundEmitter e; e.Dead = 0; e.Playing = 7; e.Last = 0;
    CTCSoundPlayer s; s.PAttachedEmitter.PItem = &e; s.PAttachedEmitter.Fetches = 0;
    if (s.IsAttachedSoundLoadedAndPlaying(7)) FAIL;
    g_pSoundSystem = &sys;
    if (!s.IsAttachedSoundLoadedAndPlaying(7) || s.PAttachedEmitter.Fetches != 3) FAIL;
    if (s.IsAttachedSoundLoadedAndPlaying(8)) FAIL;
    e.Dead = 1;
    if (s.IsAttachedSoundLoadedAndPlaying(7)) FAIL;
    e.Dead = 0; s.PAttachedEmitter.PItem = 0;
    if (s.IsAttachedSoundLoadedAndPlaying(7)) FAIL;
    std::printf("0066ae60_TEST PASS\n");
    return 0;
}
