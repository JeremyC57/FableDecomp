// NParticleEngine::CPSCEmitterGeneric::ChangeOrientation(const C3DVector& v) -- remaps an emitter-space direction for the XZ or YZ emitter orientation (swapping axes and negating Z), unchanged for the default XY orientation
// Leaf: ?ChangeOrientation@CPSCEmitterGeneric@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B10360, length 136 bytes, thiscall `ret 8` (hidden result pointer, &v),
//   the result pointer returned in eax.
// Signature from Ego_r.pdb: ?ChangeOrientation@CPSCEmitterGeneric@NParticleEngine@@AAE?AVC3DVector@@ABV3@@Z (private).
//   Callers: CPSCEmitterGeneric::GenerateParticle @ 0x00B10B10 (seven calls).
// Callees: none.
// Layout (Ego_r.pdb): the emitter's first bitfield dword at +0x30 = NoParticlesToStart:10, NoParticlesToStartRand:10,
//   EmitterType:2, Solid:1, UseEmitterLifeSecs:1, UseEmitterTimelineSecs:1, OrientationXY:1, OrientationXZ:1 (bit 26,
//   0x4000000), OrientationYZ:1 (bit 27, 0x8000000), ...
// Shape: a named result: `C3DVector result; if (OrientationXZ) result = C3DVector(v.X, -v.Z, v.Y); else if
//   (OrientationYZ) result = C3DVector(-v.Z, v.X, v.Y); else result = v; return result;` -- each remap built in a
//   temporary (the negation by fchs), copied into the local, then copied out.  C3DVector's memberwise user copy
//   constructor keeps the local in the frame (implicit copy: the result lives in ecx/edx/esi, DIFFER 113v136).
// Label: catalog "CParticleDataSource_ApplyAxisRotation" is behavioural.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NParticleEngine {

class CPSCEmitterGeneric {
public:
    unsigned char _pad_0x00[0x30];
    unsigned long NoParticlesToStart : 10;                  // +0x30 bits 0..9
    unsigned long NoParticlesToStartRand : 10;              //        bits 10..19
    unsigned long EmitterType : 2;                          //        bits 20..21
    unsigned long Solid : 1;                                //        bit 22
    unsigned long UseEmitterLifeSecs : 1;                   //        bit 23
    unsigned long UseEmitterTimelineSecs : 1;               //        bit 24
    unsigned long OrientationXY : 1;                        //        bit 25
    unsigned long OrientationXZ : 1;                        //        bit 26
    unsigned long OrientationYZ : 1;                        //        bit 27
    unsigned long UseCustomDirection : 1;                   //        bit 28
    unsigned long UseOutwardDirection : 1;                  //        bit 29
    unsigned long UseParamDirection : 1;                    //        bit 30
    unsigned long OppositeDirection : 1;                    //        bit 31
private:
    C3DVector ChangeOrientation(const C3DVector& v);
};

C3DVector CPSCEmitterGeneric::ChangeOrientation(const C3DVector& v)
{
    C3DVector result;
    if (OrientationXZ)
        result = C3DVector(v.X, -v.Z, v.Y);
    else if (OrientationYZ)
        result = C3DVector(-v.Z, v.X, v.Y);
    else
        result = v;
    return result;
}

} // namespace NParticleEngine