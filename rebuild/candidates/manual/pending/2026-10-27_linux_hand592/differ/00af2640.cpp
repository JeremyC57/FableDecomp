// NParticleEngine::CPSCBase::GetCurrentPosition(const CParticleUpdateParameters<unsigned long,unsigned long>& params, const unsigned long& param, float timeOffset, bool local, unsigned long flags) const -- the component's current position in world space: the update parameter for `param`, else the component's user point, else the origin; moved through the system transform when the particle system works in local space
// Grade: DIFFER(298v313) -- frame 0x3C+edi v 0x54; retail stores each transformed row straight into x/y's home slots with x and y preloaded on the FPU (fld [esp]; fld [esp+4], later fmul st(3)/st(2)) and builds the C3DVector result temp (copy ctor) before `pos = T`; this form computes all three rows first. Grade 3 (memberwise inline CMatrix3x4::operator* + user C3DVector copy ctor, `pos = C3DVector(v)`) got retail's return copy (fld X / fstp) and the T temp but kept a separate r temp (341v313).
// Leaf: ?GetCurrentPosition@CPSCBase@NParticleEngine@@QBE?AVC3DVector@@ABV?$CParticleUpdateParameters@KK@2@ABKM_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2640, 313 bytes, thiscall `ret 18h` (hidden C3DVector result + five arguments).
// Identity (proven, two sources): Ego_r.pdb names CPSCBase::GetCurrentPosition and GetCurrentPositionLocal with exactly
//   this signature (QBE, `?AVC3DVector@@ ABV?$CParticleUpdateParameters@KK@2@ ABK M _N K`); this row and its twin
//   0x00AF2780 differ only in the transform: this one applies CParticleSystem::GetSystemTransform 0x00AF2250 (the
//   emitter's Transform, landed under that PDB spelling) when the system is local (+0x50 bit 6) -- local to world, the
//   world-space getter -- while 0x00AF2780 applies GetInverseSystemTransform 0x00AF22C0 when it is NOT local (world to
//   local: GetCurrentPositionLocal). The landed CPSCLight::AddToBoundingSphere 0x00B18AF0 calls this row as
//   GetCurrentPosition (callers also 0x00B17A2A, 0x00B1A414). NAME_FIXES.tsv: 0x00AF2250 is bound there as
//   GetInverseSystemTransform, 0x00AF2780 is bound by 0x00B16D50 as GetCurrentPosition, 0x00AF3080 without its namespace.
// Callees: CParticleUpdateParameters<unsigned long,unsigned long>::GetUpdateParam(C3DVector&, const unsigned long&, float,
//   bool, unsigned long) const 0x00AF3080, virtual slot 30 GetUserPoint(C3DVector&) const, GetSystemTransform 0x00AF2250.
// Layout: Ego_r.pdb CPSCBase (12 bytes, retail offsets equal): vptr, ParticleSystem +4, UpdateStarted/Initialised +8.
//   The vtable 0x0129F3F4 (32 slots) is modelled with the PDB names its derived classes' slots carry (rtti-vtables + the
//   landed CPSC* rows); slots 4/5 return constants in every derived class and have no name in either PDB set.
// Shape: the origin fallback is a chained assignment (stores z, y, x); the transform is the FableWin.pdb inline
//   CMatrix3x4::operator*(const C3DVector&) const.
namespace NParticleEngine { class CParticleSystem; class CParticleSystemCost; class CEmitterDatabaseSerialiseInfo;
    template <class K, class V> class CParticleUpdateParameters; }
class CCharString { public: void* PStringData; };
class CSymbolMap; class CDataInputStream; class CDataOutputStream; class CPersistContext; class CIEngine;
class CEnginePrimitiveHandle; class CEnginePrimitivePrepareInfo; class C3DBoundingSphere;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    C3DVector operator*(const C3DVector& v) const    // FableWin.pdb ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z (inline)
    {
        return C3DVector(v.X * E11 + v.Y * E21 + v.Z * E31 + E41,
                         v.X * E12 + v.Y * E22 + v.Z * E32 + E42,
                         v.X * E13 + v.Y * E23 + v.Z * E33 + E43);
    }
};

namespace NParticleEngine {

template <class K, class V> class CParticleUpdateParameters {
public:
    bool GetUpdateParam(C3DVector& out, const K& key, float timeOffset, bool local, V flags) const;   // 0x00AF3080
};

class CParticleSystem {
public:
    unsigned char _pad00[0x50];
    unsigned char Flags;                        // +0x50 (bit 6: local space)
    void GetSystemTransform(CMatrix3x4& matrix) const;            // 0x00AF2250
};

template <class K, class V> class CParticleUpdateParamCallback {
public:
    virtual ~CParticleUpdateParamCallback();                                                       // slot 0
    virtual bool GetUpdateParam(C3DVector& out, K key, float timeOffset, bool local,
                                const CParticleUpdateParameters<K, V>& params, V flags);           // slot 1
};

typedef CParticleUpdateParameters<unsigned long, unsigned long> CPSCParams;

class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    virtual ~CPSCBase();
    virtual CPSCBase* DatabaseClone() const = 0;                                                    // slot 2
    virtual CCharString GetClassTypeName() const = 0;                                               // slot 3
    virtual long CUnprovenConstSlot04() = 0;                                                        // slot 4 (unnamed)
    virtual long CUnprovenConstSlot05() = 0;                                                        // slot 5 (unnamed)
    virtual CCharString GetGUIName() const = 0;                                                     // slot 6
    virtual void OnCreate();                                                                        // slot 7
    virtual void StartUpdate(const CPSCParams& params);                                             // slot 8
    virtual void Update(CParticleSystem& system, const CPSCParams& params, bool& alive);            // slot 9
    virtual void CollectUpdateParameters(CPSCParams& params, const unsigned long& prefix);         // slot 10
    virtual void TransferUpdateParametersToDialog(const CSymbolMap& symbols);                       // slot 11
    virtual void UpdateAddParticle(CParticleSystem& system, const CPSCParams& params, C3DVector pos,
                                   C3DVector vel, float timeOffset);                               // slot 12
    virtual void PreparePrimitives(float timeOffset, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CPSCParams& params, CEnginePrimitiveHandle& handle);       // slot 13
    virtual void InterpolateTo(const CPSCBase& other, float t) = 0;                                 // slot 14
    virtual void ReadBinary(CDataInputStream& stream) = 0;                                          // slot 15
    virtual void WriteBinary(CDataOutputStream& stream) const = 0;                                  // slot 16
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);   // slot 17
    virtual void DatabaseCopy(const CPSCBase& other) = 0;                                           // slot 18
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system,
                                     const CPSCParams& params);                                     // slot 19
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float a, float b) = 0;  // slot 20
    virtual bool GetParticleLifeSecs(float& secs);                                                  // slot 21
    virtual bool GetNumberOfParticles(float& count, float timeOffset);                              // slot 22
    virtual bool GetDecalLifeSecs(float& secs);                                                     // slot 23
    virtual bool GetNumberOfDecals(float& count, float timeOffset);                                 // slot 24
    virtual void EnterTimelineEnd();                                                                // slot 25
    virtual long GetParticleCount();                                                                // slot 26
    virtual long GetDecalCount();                                                                   // slot 27
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system, const CPSCParams& params,
                            CEnginePrimitiveHandle& handle);                                        // slot 28
    virtual void ClearAllPrimitives(CIEngine* engine, CEnginePrimitiveHandle& handle);             // slot 29
    virtual bool GetUserPoint(C3DVector& point) const;                                              // slot 30
    virtual float GetApproximateSplineLength();                                                     // slot 31

    C3DVector GetCurrentPosition(const CPSCParams& params, const unsigned long& param, float timeOffset, bool local,
                                 unsigned long flags) const;

    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
};

C3DVector CPSCBase::GetCurrentPosition(const CPSCParams& params, const unsigned long& param, float timeOffset, bool local,
                                       unsigned long flags) const
{
    C3DVector pos;
    if (!params.GetUpdateParam(pos, param, timeOffset, local, flags) && !GetUserPoint(pos))
        pos.X = pos.Y = pos.Z = 0.0f;
    if (ParticleSystem) {
        float x = pos.X;
        float y = pos.Y;
        float z = pos.Z;
        if (ParticleSystem->Flags & 0x40) {
            CMatrix3x4 m;
            ParticleSystem->GetSystemTransform(m);
            float nx = m.E31 * z + m.E21 * y + m.E11 * x + m.E41;
            float ny = m.E32 * z + m.E22 * y + m.E12 * x + m.E42;
            float nz = m.E33 * z + m.E23 * y + m.E13 * x + m.E43;
            x = nx;
            y = ny;
            z = nz;
        }
        pos = C3DVector(x, y, z);
    }
    return pos;
}

}
