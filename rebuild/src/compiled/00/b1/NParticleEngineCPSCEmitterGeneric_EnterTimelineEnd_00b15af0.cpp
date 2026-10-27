// NParticleEngine::CPSCEmitterGeneric::EnterTimelineEnd() -- the emitter's timeline has ended: stop timing the emitter against the timeline
// Leaf: ?EnterTimelineEnd@CPSCEmitterGeneric@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15AF0, 8 bytes. Slot 25 (+0x64) of the CPSCEmitterGeneric vtable 0x012A002C
//   (absolute ref 0x012A0090); landed component models name slot 25 EnterTimelineEnd, and Ego_r.pdb has
//   `?EnterTimelineEnd@CPSCEmitterGeneric@NParticleEngine@@UAEXXZ`.
// Layout: Ego_r.pdb CPSCEmitterGeneric: unsigned long bit fields from +0x30 -- NoParticlesToStart:10,
//   NoParticlesToStartRand:10, EmitterType:2, Solid:1, UseEmitterLifeSecs:1, UseEmitterTimelineSecs:1 (bit 24), ... --
//   so clearing UseEmitterTimelineSecs is `and dword ptr [ecx+30h],0FEFFFFFFh`.
namespace NParticleEngine {

class CPSCBase {
public:
    void*         ParticleSystem;                          // +0x04
    unsigned char Flags;                                   // +0x08 (UpdateStarted / Initialised bits)
    virtual void EnterTimelineEnd();                       // slot 25 (+0x64)
};

class CPSCEmitterGeneric : public CPSCBase {
public:
    unsigned char EmitterPosParamDesc[0xC];                // +0x0C CParticleUpdateParamDesc<ulong,ulong>
    unsigned char Spline[0x8];                             // +0x18 CCountedPointer<CCardinalSpline>
    unsigned long EmitterPosParam;                         // +0x20
    unsigned long DirectionParamName;                      // +0x24
    long          LifeTimer;                               // +0x28
    float         NextGenerateTime;                        // +0x2C
    unsigned long NoParticlesToStart : 10;                 // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;              // bit 24
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    virtual void EnterTimelineEnd();
};

void CPSCEmitterGeneric::EnterTimelineEnd()
{
    UseEmitterTimelineSecs = 0;
}

}   // namespace NParticleEngine