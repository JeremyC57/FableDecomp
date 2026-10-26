// Behaviour test for 0x0066ADD0 CTCSoundPlayer::StopAttachedSound: nothing without the sound system, without an attached
// emitter or with a dead emitter; otherwise the emitter's StopSpecificSample for the sample id.
#include <cstdio>
#define FAIL do { std::printf("0066add0_TEST FAIL\n"); return 1; } while (0)
void* g_pSoundSystem = 0;
class CThingSoundEmitter {
public:
    unsigned char Dead;
    unsigned long Playing, Last;
    void StopSpecificSample(unsigned long sampleID) { Last = sampleID; }
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
    void StopAttachedSound(unsigned long sampleID);
};
void CTCSoundPlayer::StopAttachedSound(unsigned long sampleID)
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!PAttachedEmitter.GetPItem()->Dead)
                PAttachedEmitter.GetPItem()->StopSpecificSample(sampleID);
        }
    }
}
int main()
{
    int sys = 1;
    CThingSoundEmitter e; e.Dead = 0; e.Playing = 7; e.Last = 0;
    CTCSoundPlayer s; s.PAttachedEmitter.PItem = &e; s.PAttachedEmitter.Fetches = 0;
    s.StopAttachedSound(7); if (e.Last != 0) FAIL;
    g_pSoundSystem = &sys;
    s.StopAttachedSound(7); if (e.Last != 7 || s.PAttachedEmitter.Fetches != 3) FAIL;
    e.Dead = 1; e.Last = 0;
    s.StopAttachedSound(9); if (e.Last != 0) FAIL;
    e.Dead = 0; s.PAttachedEmitter.PItem = 0;
    s.StopAttachedSound(9); if (e.Last != 0) FAIL;
    std::printf("0066add0_TEST PASS\n");
    return 0;
}
