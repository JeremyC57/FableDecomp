// CParticleCreationParameters::CParticleCreationParameters(long, long, float, const CRightHandedSet&) -- interpolated creation record: InterpolatingParticle set, the particle-ID range and interpolation factor stored, RHSet copied, empty update parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2640, length 127 bytes (padded to 128).
// Leaf: ??0CParticleCreationParameters@@QAE@JJM
// Callees: CBase::CBase() @ 0x0099A2D0 (base of NParticleEngine::CParticleUpdateParameters, vtable 0x0125FFDC).
// Layout: as 0x006E25C0 (retail UpdateParameters 0x24 bytes at +0x28).
// Shape: initialiser list in declaration order (the compiler schedules the three argument stores first); the RHSet
// is copied through ecx/edx by C3DVector's memberwise user copy constructor.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet {
    C3DVector Up;                           // +0x00
    C3DVector Forward;                      // +0x0c
};
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;
namespace NParticleEngine {
struct SVectorParam;                        // pair<unsigned long, C3DVector>
struct SUpdateParam;                        // pair<unsigned long, CParticleUpdateParamDesc>
template <class T> struct CParamVector {    // retail STLport vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    CParamVector() : m_first(0), m_last(0), m_end(0) {}
};
struct CCompareVector { };
struct CCompareParamDesc { };
struct CVectorParamMap {                    // CVectorMap<unsigned long, C3DVector, ...> (16 bytes)
    CParamVector<SVectorParam> Vector;      // +0x00
    CCompareVector Compare;                 // +0x0c
    bool Dirty;                             // +0x0d
    CVectorParamMap() : Dirty(false) {}
};
struct CUpdateParamMap {                    // CVectorMap<unsigned long, CParticleUpdateParamDesc, ...> (16 bytes)
    CParamVector<SUpdateParam> Vector;      // +0x00
    CCompareParamDesc Compare;              // +0x0c
    bool Dirty;                             // +0x0d
    CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBase {  // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;           // +0x04
    CUpdateParamMap UpdateParams;           // +0x14
    CParticleUpdateParameters() {}
};
}
struct CParticleCreationParameters {        // Ego_r.pdb, 0x54 bytes (retail 0x50: 12-byte vectors)
    bool  InterpolatingParticle;            // +0x00
    bool  ParticleCreationDisabled;         // +0x01
    bool  UpdateParametersSet;              // +0x02
    bool  AlwaysUpdateFlag;                 // +0x03
    long  StartParticleID;                  // +0x04
    long  EndParticleID;                    // +0x08
    float Interpolate;                      // +0x0c
    CRightHandedSet RHSet;                  // +0x10
    NParticleEngine::CParticleUpdateParameters UpdateParameters;   // +0x28
    CParticleCreationParameters(long particleID, const CRightHandedSet& rhSet);
    CParticleCreationParameters(long startID, long endID, float interpolate, const CRightHandedSet& rhSet);
};
CParticleCreationParameters::CParticleCreationParameters(long startID, long endID, float interpolate, const CRightHandedSet& rhSet)
    : InterpolatingParticle(true), ParticleCreationDisabled(false), UpdateParametersSet(false), AlwaysUpdateFlag(false),
      StartParticleID(startID), EndParticleID(endID), Interpolate(interpolate), RHSet(rhSet)
{
}