// NParticleEngine::CPSCEmitterGeneric::WriteBinary(CDataOutputStream*) -- serialize the generic emitter: its params, the packed bit-field integers/flags, then the optional cardinal spline (tension, point count, points)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B12EF0, 742 bytes (vtable slot 16 of CPSCEmitterGeneric, 0x012A002C).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteULong 0x00993EA0 (propagated
//   label "WriteCBYTE"), WriteEBOOL 0x00993E30, WriteFloat 0x00993EE0, Write3DVector 0x00993FD0,
//   vector<C3DVector> copy constructor 0x006F0810 (as declared by the landed CTCShapeManager::InitialiseFrom
//   0x00762AB0), free 0x00BFEA14 (the copy's storage).
// Layout: PDB (Ego_r) CPSCEmitterGeneric names/types; retail offsets equal the PDB: Spline (CCountedPointer) +0x18,
//   EmitterPosParam/DirectionParamName +0x20/+0x24, unsigned long bit-field words +0x30..+0x44, the unsigned short
//   NonUniformScalingZ:11 at +0x48. CCardinalSpline: retail CArray<C3DVector> is 12 bytes (begin/end/capacity, the
//   PDB says 16), so Tension sits at +0x0c (retail reads [Data+0xc]).
// Shape: the copy constructor needs a same-TU auto_inline(off) stand-in (VC7.1 then keeps the copy's _M_start in ebp
//   across the loop and the free); one stream call per member; the spline flag is a named bool (kept in bl, stored to the dead argument slot
//   for the push); the points are written from a local copy of the control-point vector over the count read first.
#include "engine/CDataOutputStream.h"
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                        // 0x00993E30
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
    void Write3DVector(const C3DVector& value);         // 0x00993FD0
};
struct CPointVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPointVector(const CPointVector& other);            // 0x006F0810
    ~CPointVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    C3DVector& operator[](unsigned long i) { return _M_start[i]; }
};
struct CCardinalSpline {
    CPointVector ControlPoints;                         // +0x00 (12 bytes in retail)
    float Tension;                                      // +0x0c
    bool EndPointsConnected;                            // +0x10
};
struct CCountedPointerInfo;
template<class T> struct CCountedPointer {
    T* Data; CCountedPointerInfo* Info;
    T* operator->() const { return Data; }
};
#pragma auto_inline(off)
CPointVector::CPointVector(const CPointVector& other)   // same-TU stand-in for 0x006F0810: reads the source, writes only *this
{
    _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage;
}
#pragma auto_inline(on)
namespace NParticleEngine {
struct CParticleUpdateParamDesc { unsigned long Param; unsigned long Name; unsigned long Type; };
struct CPSCEmitterGeneric {
    unsigned char _base[0xc];                           // CPSCBase
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
    void WriteBinary(CDataOutputStream* out);
};
void CPSCEmitterGeneric::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteULong(EmitterPosParam);
    out->WriteULong(DirectionParamName);
    out->WriteULong(NoParticlesToStart);
    out->WriteULong(NoParticlesToStartRand);
    out->WriteULong(EmitterType);
    out->WriteEBOOL(Solid);
    out->WriteEBOOL(UseEmitterLifeSecs);
    out->WriteEBOOL(UseEmitterTimelineSecs);
    out->WriteEBOOL(OrientationXY);
    out->WriteEBOOL(OrientationXZ);
    out->WriteEBOOL(OrientationYZ);
    out->WriteEBOOL(UseCustomDirection);
    out->WriteEBOOL(UseOutwardDirection);
    out->WriteEBOOL(UseParamDirection);
    out->WriteEBOOL(OppositeDirection);
    out->WriteULong(AngularPerturbationInteger);
    out->WriteULong(CustomDirectionX);
    out->WriteULong(ParticlesPerSecond);
    out->WriteULong(EmitterSize);
    out->WriteULong(RadialBias);
    out->WriteULong(MinSpeed);
    out->WriteEBOOL(UseForwardDirection);
    out->WriteEBOOL(UseRandom2DDirection);
    out->WriteULong(CustomDirectionY);
    out->WriteULong(CustomDirectionZ);
    out->WriteULong(EmitterTimelineSecs);
    out->WriteEBOOL(UseRandom3DDirection);
    out->WriteULong(EmitterLifeSecs);
    out->WriteULong(EmitterStartTime);
    out->WriteULong(MaxSpeed);
    out->WriteULong(NonUniformScalingX);
    out->WriteULong(NonUniformScalingY);
    out->WriteULong(NonUniformScalingZ);
    bool hasSpline = Spline.Data != 0;
    out->WriteEBOOL(hasSpline);
    if (hasSpline) {
        out->WriteFloat(Spline->Tension);
        unsigned long count = Spline->ControlPoints.size();
        out->WriteULong(count);
        CPointVector points(Spline->ControlPoints);
        for (unsigned long i = 0; i < count; ++i)
            out->Write3DVector(points._M_start[i]);
    }
}
}
