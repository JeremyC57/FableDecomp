// NParticleEngine::CPSCEmitterGeneric::ReadBinary(CDataInputStream& in) -- vtable slot 15: read the generic emitter back in WriteBinary order (param keys, bit-field integers and flags, then the optional cardinal spline)
// Leaf: ?ReadBinary@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B149D0, 3779 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the
//   CPSCEmitterGeneric vtable 0x012A002C (slot 15 = ReadBinary family-wide, chunk 737); the mirror of the landed WriteBinary 0x00B12EF0.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inlined Read, x36), Read3DVector 0x009950F0,
//   operator new 0x00BFEA1A (the CCardinalSpline, 0x14 bytes; its default ctor inline: no points, Tension 0.5, open ends),
//   CCountedPointer<CCardinalSpline>::Reset 0x0075FA30 (FableWin ?Reset@?$CCountedPointer@VCCardinalSpline@@@@QAEXPAVCCardinalSpline@@@Z),
//   STLport vector<C3DVector>(size_type) 0x007C2AB0 (CArray<C3DVector>(n) forwards to it, engine/CArray.h FABLE_RETAIL_CARRAY),
//   vector<C3DVector>::operator= 0x0075B8D0, free 0x00BFEA14 (the local array's storage).
// Layout: the landed WriteBinary / DatabaseCopy model (retail offsets = Ego_r PDB for this class): Spline +0x18, the two param keys
//   +0x20 / +0x24, the unsigned long bit-field words +0x30..+0x44, the unsigned short NonUniformScalingZ:11 at +0x48; CCardinalSpline
//   ControlPoints +0 (12-byte STLport vector), Tension +0xc, EndPointsConnected +0x10.
// Shape: one inline read per member in WriteBinary order; the spline points are read into a local CArray and assigned in one go.
//   stlport/fable_containers.h FABLE_RETAIL_STLP_VECTOR_C makes STLport's _vector.c operator= body visible (as in retail, where
//   _vector.h includes _vector.c): VC7.1 then knows the out-of-line operator= does not capture the local array and keeps its start
//   pointer in ebx from the loop to the free (declaration-only operator= reloads it: 3784v3779).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                    // +0x00 (12 bytes in retail)
    float Tension;                                      // +0x0c
    bool EndPointsConnected;                            // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
};
class CParticleUpdateParamDesc { public: unsigned long Param; unsigned long Name; unsigned long Type; };
class CPSCEmitterGeneric : public CPSCBase { public:
    CParticleUpdateParamDesc EmitterPosParamDesc;       // +0x0c
    CCountedPointer<CCardinalSpline> Spline;            // +0x18
    unsigned long EmitterPosParam;                      // +0x20
    unsigned long DirectionParamName;                   // +0x24
    long LifeTimer;                                     // +0x28
    float NextGenerateTime;                             // +0x2c
    unsigned long NoParticlesToStart : 10;              // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;      // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                     // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                 // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                 // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                        // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;             // +0x48
    unsigned long StartedTimer : 1;                     // +0x4c
    unsigned long FinishedGenerating : 1;
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCEmitterGeneric::ReadBinary(CDataInputStream& in)
{
    EmitterPosParam = in.ReadULONG();
    DirectionParamName = in.ReadULONG();
    NoParticlesToStart = in.ReadULONG();
    NoParticlesToStartRand = in.ReadULONG();
    EmitterType = in.ReadULONG();
    Solid = in.ReadEBOOL();
    UseEmitterLifeSecs = in.ReadEBOOL();
    UseEmitterTimelineSecs = in.ReadEBOOL();
    OrientationXY = in.ReadEBOOL();
    OrientationXZ = in.ReadEBOOL();
    OrientationYZ = in.ReadEBOOL();
    UseCustomDirection = in.ReadEBOOL();
    UseOutwardDirection = in.ReadEBOOL();
    UseParamDirection = in.ReadEBOOL();
    OppositeDirection = in.ReadEBOOL();
    AngularPerturbationInteger = in.ReadULONG();
    CustomDirectionX = in.ReadULONG();
    ParticlesPerSecond = in.ReadULONG();
    EmitterSize = in.ReadULONG();
    RadialBias = in.ReadULONG();
    MinSpeed = in.ReadULONG();
    UseForwardDirection = in.ReadEBOOL();
    UseRandom2DDirection = in.ReadEBOOL();
    CustomDirectionY = in.ReadULONG();
    CustomDirectionZ = in.ReadULONG();
    EmitterTimelineSecs = in.ReadULONG();
    UseRandom3DDirection = in.ReadEBOOL();
    EmitterLifeSecs = in.ReadULONG();
    EmitterStartTime = in.ReadULONG();
    MaxSpeed = in.ReadULONG();
    NonUniformScalingX = in.ReadULONG();
    NonUniformScalingY = in.ReadULONG();
    NonUniformScalingZ = in.ReadULONG();
    bool hasSpline = in.ReadEBOOL();
    if (hasSpline) {
        Spline.Reset(new CCardinalSpline);
        Spline->Tension = in.ReadFloat();
        unsigned long count = in.ReadULONG();
        CArray<C3DVector> points(count);
        for (unsigned long i = 0; i < count; ++i)
            points[i] = in.Read3DVector();
        Spline->ControlPoints = points;
    }
}
}