// Behaviour test for 0x00B15AF0 NParticleEngine::CPSCEmitterGeneric::EnterTimelineEnd() -- clears UseEmitterTimelineSecs only
#include <cstdio>
class CPSCEmitterGeneric {
public:
    unsigned long NoParticlesToStart : 10;
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OppositeDirection : 1;
    virtual ~CPSCEmitterGeneric() { }
    virtual void EnterTimelineEnd() { UseEmitterTimelineSecs = 0; }
};
int main()
{
    CPSCEmitterGeneric e;
    e.NoParticlesToStart = 1000; e.NoParticlesToStartRand = 3; e.EmitterType = 2; e.Solid = 1;
    e.UseEmitterLifeSecs = 1; e.UseEmitterTimelineSecs = 1; e.OrientationXY = 1; e.OppositeDirection = 1;
    e.EnterTimelineEnd();
    if (e.UseEmitterTimelineSecs != 0) { std::printf("00b15af0_TEST FAIL\n"); return 1; }
    if (e.NoParticlesToStart != 1000 || e.NoParticlesToStartRand != 3 || e.EmitterType != 2 || e.Solid != 1 ||
        e.UseEmitterLifeSecs != 1 || e.OrientationXY != 1 || e.OppositeDirection != 1) { std::printf("00b15af0_TEST FAIL\n"); return 1; }
    std::printf("00b15af0_TEST PASS\n");
    return 0;
}