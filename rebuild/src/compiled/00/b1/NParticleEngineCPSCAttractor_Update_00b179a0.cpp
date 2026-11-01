// NParticleEngine::CPSCAttractor::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool&) -- pull every active particle inside the attractor's influence radius towards the attractor position (a named vector parameter or the component's current position) with a force that falls off with distance; unless the system uses all attractors only the closest one so far applies
// Leaf: ?Update@CPSCAttractor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B179A0, 554 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb `?Update@CPSCAttractor@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z`;
//   vtable 0x012A04AC slot 9 (Update on the landed CPSCLight / CPSCSingleSprite / CPSCDecalRenderer vtables); no direct
//   callers (virtual). The catalog's "_global::Particle_ApplyDistanceForces" is a descriptive stand-in.
// Callees: CVectorMap<unsigned long, C3DVector, CParticleUpdateCCompareVector<unsigned long>>::LowerBound(const unsigned long&)
//   const 0x00B05890 (the inlined Find of the vector-parameter map), CPSCBase::GetCurrentPosition(params, const unsigned long&,
//   float, bool, unsigned long) const 0x00AF2640 (hidden C3DVector result), _CIpow 0x00BFEB40 (the `pow` intrinsic helper).
// Globals: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 0.2f @ 0x012305A0.
// Layout (Ego_r.pdb, retail equal): CPSCAttractor AttractorEnabled / AttractorUseParamPosition bits 0-1 of +0xc,
//   AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14, AttractorPositionParamName +0x18, AttractorInfluenceRadius
//   +0x1c, AttractorInfluenceForce +0x20. CParticle: PositionBuffer[0] +0x29, Acceleration +0x65, Active +0x7a,
//   ClosestAttractorDistance +0x7c (pack(1)). CParticleSystem: Particles (STLport list<CParticle*> header pointer) at retail
//   +0x1c (PDB +0x20), the flag byte at retail +0x50 (PDB +0x6c) bit 4 = UseAllAttractors. CParticleUpdateParameters:
//   VectorParams +0x4 (CVectorMap: Data vector +0, Compare +0xc, Dirty +0xd).
// Shape: the three gates (`AttractorEnabled`, radius not negative, force non-zero) exit early; the position is copied from
//   the found map entry or from the GetCurrentPosition result through one merged three-dword copy; the attractor force is
//   homed in the dead params slot; the falloff exponent is `(FallOff - 5)` or `FallOff * 0.2f` (int to float through the
//   dead distance slot) and the distance ratio fabs(radius - distance) / radius goes through `pow`.
extern "C" double __cdecl pow(double x, double y);     // _CIpow 0x00BFEB40
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(pow, sqrt, fabs)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }   // ??GC3DVector@@QBE?BV0@ABV0@@Z
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }                   // ??DC3DVector@@QBE?BV0@M@Z
    const C3DVector& operator*=(float f) { X = f * X; Y = f * Y; Z = f * Z; return *this; }                        // ??XC3DVector@@QAEABV0@M@Z
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }                               // ?GetMagnitude@C3DVector@@QBEMXZ
    const C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }      // ??YC3DVector@@QAEABV0@ABV0@@Z
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class vector {         // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                // +0x00
    TCompare           Compare;                             // +0x0C (empty comparator)
    bool               Dirty;                               // +0x0D

    const_iterator end() const { return Data.end(); }
    const_iterator LowerBound(const K& key) const;          // 0x00B05890
    __forceinline const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it))
            return last;
        return it;
    }
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;                                    // +0x08
};

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareVector {
        bool operator()(const K& key, const std::pair<K, C3DVector>& entry) const { return key < entry.first; }
    };
    typedef CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> > CVectorParamMap;

    virtual ~CParticleUpdateParameters();
    CVectorParamMap VectorParams;                           // +0x04
};

#pragma pack(push, 1)
class CParticle { public:
    unsigned char _pad_0x00[0x29];
    C3DVector     PositionBuffer[4];                        // +0x29
    C3DVector     Velocity;                                 // +0x59
    C3DVector     Acceleration;                             // +0x65
    unsigned char _pad_0x71[0x9];
    bool          Active;                                   // +0x7a
    unsigned char _pad_0x7b;
    float         ClosestAttractorDistance;                 // +0x7c
};
#pragma pack(pop)

class CParticleSystem { public:
    unsigned char           _pad_0x00[0x1c];
    _List_node<CParticle*>* Particles;                      // +0x1c list<CParticle*> header node (retail)
    unsigned char           _pad_0x20[0x30];
    bool                    Initialised : 1;                // +0x50 bit 0
    bool                    Enabled : 1;
    bool                    Dead : 1;
    bool                    NormalisedTimeEnabled : 1;
    bool                    UseAllAttractors : 1;           //        bit 4
    bool                    Visible : 1;
    bool                    StayWithEmitter : 1;
    bool                    ScaleParticles : 1;
};

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;                        // +0x04
    bool             UpdateStarted : 1;                     // +0x08 bit 0
    bool             Initialised : 1;                       // +0x08 bit 1
    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                 const unsigned long& param, float timeOffset, bool local, unsigned long flags) const;  // 0x00AF2640
};

class CPSCAttractor : public CPSCBase { public:            // vtable 0x012A04AC
    bool           AttractorEnabled : 1;                    // +0x0c bit 0
    bool           AttractorUseParamPosition : 1;           // +0x0c bit 1
    long           AttractorInfluenceFallOff;               // +0x10
    unsigned long  AttractorPositionParam;                  // +0x14
    unsigned long  AttractorPositionParamName;              // +0x18
    float          AttractorInfluenceRadius;                // +0x1c
    float          AttractorInfluenceForce;                 // +0x20
    CArray<C3DVector> AttractorUserPointsArray;             // +0x24
    virtual void Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                        bool& changed);
};

void CPSCAttractor::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                           bool& changed)
{
    if (AttractorEnabled && AttractorInfluenceRadius >= 0.0f && AttractorInfluenceForce != 0.0f) {
        C3DVector position;
        CParticleUpdateParameters<unsigned long, unsigned long>::CVectorParamMap::const_iterator it;
        if (AttractorUseParamPosition && (it = params.VectorParams.Find(AttractorPositionParamName)) != params.VectorParams.end())
            position = it->second;
        else
            position = GetCurrentPosition(params, AttractorPositionParam, 1.0f, false, 0);

        bool useAllAttractors = system.UseAllAttractors;
        for (_List_node<CParticle*>* node = system.Particles->_M_next; node != system.Particles; node = node->_M_next) {
            CParticle* particle = node->_M_data;
            if (!particle->Active)
                continue;
            C3DVector delta(position.X - particle->PositionBuffer[0].X, position.Y - particle->PositionBuffer[0].Y,
                            position.Z - particle->PositionBuffer[0].Z);
            float distance = delta.GetMagnitude();
            if (AttractorInfluenceRadius > distance) {
                if (AttractorInfluenceRadius <= particle->ClosestAttractorDistance || useAllAttractors) {
                    delta *= 1.0f / distance;
                    particle->ClosestAttractorDistance = AttractorInfluenceRadius;
                    C3DVector acceleration = delta * AttractorInfluenceForce;
                    float ratio = (float)fabs(AttractorInfluenceRadius - distance) / AttractorInfluenceRadius;
                    float exponent;
                    if (AttractorInfluenceFallOff <= 5)
                        exponent = (float)AttractorInfluenceFallOff * 0.2f;
                    else
                        exponent = (float)(AttractorInfluenceFallOff - 5);
                    acceleration *= (float)pow(ratio, exponent);
                    if (!useAllAttractors)
                        particle->Acceleration.X = particle->Acceleration.Y = particle->Acceleration.Z = 0.0f;
                    particle->Acceleration += acceleration;
                }
            }
        }
    }
}

}   // namespace NParticleEngine