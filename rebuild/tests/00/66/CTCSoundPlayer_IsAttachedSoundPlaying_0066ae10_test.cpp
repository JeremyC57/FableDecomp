// Behaviour test for 0x0066AE10 (CTCSoundPlayer::IsAttachedSoundPlaying): false without the sound system, without
// an attached emitter, or with a dead emitter; otherwise the emitter's answer for the sample id.
#include <cstdio>

void* g_pSoundSystem = 0;

class CThingSoundEmitter {
public:
    unsigned char Dead;
    unsigned long Playing;
    bool IsSamplePlaying(unsigned long sampleID) const { return sampleID == Playing; }
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
    bool IsAttachedSoundPlaying(unsigned long sampleID) const;
};

bool CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long sampleID) const
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!PAttachedEmitter.GetPItem()->Dead)
                return PAttachedEmitter.GetPItem()->IsSamplePlaying(sampleID);
        }
    }
    return false;
}

int main()
{
    int sys = 1;
    CThingSoundEmitter e;
    e.Dead = 0; e.Playing = 7;
    CTCSoundPlayer s;
    s.PAttachedEmitter.PItem = &e; s.PAttachedEmitter.Fetches = 0;
    if (s.IsAttachedSoundPlaying(7)) { std::printf("0066ae10_TEST FAIL\n"); return 1; }
    g_pSoundSystem = &sys;
    if (!s.IsAttachedSoundPlaying(7) || s.PAttachedEmitter.Fetches != 3) { std::printf("0066ae10_TEST FAIL\n"); return 1; }
    if (s.IsAttachedSoundPlaying(8)) { std::printf("0066ae10_TEST FAIL\n"); return 1; }
    e.Dead = 1;
    if (s.IsAttachedSoundPlaying(7)) { std::printf("0066ae10_TEST FAIL\n"); return 1; }
    e.Dead = 0;
    s.PAttachedEmitter.PItem = 0;
    if (s.IsAttachedSoundPlaying(7)) { std::printf("0066ae10_TEST FAIL\n"); return 1; }
    std::printf("0066ae10_TEST PASS\n");
    return 0;
}
