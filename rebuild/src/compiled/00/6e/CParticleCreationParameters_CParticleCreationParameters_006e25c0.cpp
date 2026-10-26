// CParticleCreationParameters::CParticleCreationParameters(long, const CRightHandedSet&) -- single-particle creation record: flags clear, StartParticleID = id, EndParticleID/Interpolate zero, RHSet copied, empty update parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E25C0, length 118 bytes (padded to 128).
// Leaf: ??0CParticleCreationParameters@@QAE@JABVCRightHandedSet
// Callees: CBase::CBase() @ 0x0099A2D0 (base of NParticleEngine::CParticleUpdateParameters, vtable 0x0125FFDC).
// Layout: Ego_r.pdb CParticleCreationParameters (four bools +0..+3, StartParticleID +4, EndParticleID +8, Interpolate
// +0xc, RHSet +0x10, UpdateParameters +0x28); the retail CVectorMaps are 16 bytes (12-byte vector, Compare, Dirty),
// so UpdateParameters is 0x24 bytes (VectorParams +4, UpdateParams +0x14).
// Shape: everything in the initialiser list (one shared zero register, the float zero stored as an integer); the RHSet
// is copied word by word through ecx/edx because C3DVector has a memberwise user copy constructor (the implicit one
// goes through `lea ecx` + eax); the update parameters' inline ctor: CBase out of line, vtable, then each map's vector
// (three zero words) and Dirty byte -- the empty comparator stores nothing.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public:
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
CParticleCreationParameters::CParticleCreationParameters(long particleID, const CRightHandedSet& rhSet)
    : InterpolatingParticle(false), ParticleCreationDisabled(false), UpdateParametersSet(false), AlwaysUpdateFlag(false),
      StartParticleID(particleID), EndParticleID(0), Interpolate(0.0f), RHSet(rhSet)
{
}