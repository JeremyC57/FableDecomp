// Test for 0x00B179A0 NParticleEngine::CPSCAttractor::Update: a two-particle system pulled towards a fixed attractor
// position (the parameter map lookup and the current-position fallback both exercised).
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector& operator*=(float f) { X = f * X; Y = f * Y; Z = f * Z; return *this; }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    const C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

namespace retail_std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class T> class CArray : public retail_std::vector<T, retail_std::allocator<T> > { };

template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef retail_std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data; TCompare Compare; bool Dirty;
    const_iterator end() const { return Data.end(); }
    const_iterator LowerBound(const K& key) const
    {
        const_iterator first = Data.begin(); int len = Data.end() - first;
        while (len > 0) {
            int half = len / 2; const_iterator middle = first + half;
            if (middle->first < key) { first = middle + 1; len = len - half - 1; } else len = half;
        }
        return first;
    }
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key); const_iterator last = end();
        if (it == last || Compare(key, *it)) return last;
        return it;
    }
};
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParameters {
public:
    template <class K> struct CParticleUpdateCCompareVector {
        bool operator()(const K& key, const retail_std::pair<K, C3DVector>& entry) const { return key < entry.first; }
    };
    typedef CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> > CVectorParamMap;
    virtual ~CParticleUpdateParameters() { }
    CVectorParamMap VectorParams;
};
#pragma pack(push, 1)
class CParticle { public:
    unsigned char _pad_0x00[0x29]; C3DVector PositionBuffer[4]; C3DVector Velocity; C3DVector Acceleration;
    unsigned char _pad_0x71[0x9]; bool Active; unsigned char _pad_0x7b; float ClosestAttractorDistance;
};
#pragma pack(pop)
class CParticleSystem { public:
    unsigned char _pad_0x00[0x1c]; _List_node<CParticle*>* Particles; unsigned char _pad_0x20[0x30];
    bool Initialised : 1; bool Enabled : 1; bool Dead : 1; bool NormalisedTimeEnabled : 1; bool UseAllAttractors : 1;
    bool Visible : 1; bool StayWithEmitter : 1; bool ScaleParticles : 1;
};
static C3DVector g_currentPosition(0.0f, 0.0f, 0.0f);
static int g_currentPositionCalls = 0;
class CPSCBase { public:
    virtual ~CPSCBase() { }
    CParticleSystem* ParticleSystem; bool UpdateStarted : 1; bool Initialised : 1;
    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>&, const unsigned long&, float, bool, unsigned long) const
    { ++g_currentPositionCalls; return g_currentPosition; }
};
class CPSCAttractor : public CPSCBase { public:
    bool AttractorEnabled : 1; bool AttractorUseParamPosition : 1;
    long AttractorInfluenceFallOff; unsigned long AttractorPositionParam; unsigned long AttractorPositionParamName;
    float AttractorInfluenceRadius; float AttractorInfluenceForce; CArray<C3DVector> AttractorUserPointsArray;
    virtual void Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& changed);
};

void CPSCAttractor::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& changed)
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
            if (!particle->Active) continue;
            C3DVector delta = position - particle->PositionBuffer[0];
            float distance = delta.GetMagnitude();
            if (AttractorInfluenceRadius > distance) {
                if (AttractorInfluenceRadius <= particle->ClosestAttractorDistance || useAllAttractors) {
                    delta *= 1.0f / distance;
                    particle->ClosestAttractorDistance = AttractorInfluenceRadius;
                    C3DVector acceleration = delta * AttractorInfluenceForce;
                    float ratio = (float)fabs(AttractorInfluenceRadius - distance) / AttractorInfluenceRadius;
                    float exponent;
                    if (AttractorInfluenceFallOff <= 5) exponent = (float)AttractorInfluenceFallOff * 0.2f;
                    else exponent = (float)(AttractorInfluenceFallOff - 5);
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
using namespace NParticleEngine;

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    CParticle p[2]; memset(p, 0, sizeof(p));
    _List_node<CParticle*> head, n0, n1;
    head._M_next = &n0; n0._M_next = &n1; n1._M_next = &head; head._M_prev = &n1; n1._M_prev = &n0; n0._M_prev = &head;
    n0._M_data = &p[0]; n1._M_data = &p[1];
    CParticleSystem sys; memset(&sys, 0, sizeof(sys)); sys.Particles = &head;
    p[0].Active = true; p[0].PositionBuffer[0] = C3DVector(3.0f, 0.0f, 0.0f); p[0].ClosestAttractorDistance = 100.0f;
    p[0].Acceleration = C3DVector(1.0f, 1.0f, 1.0f);
    p[1].Active = true; p[1].PositionBuffer[0] = C3DVector(50.0f, 0.0f, 0.0f); p[1].ClosestAttractorDistance = 100.0f;

    CPSCAttractor a; a.ParticleSystem = 0; a.UpdateStarted = false; a.Initialised = false; a.AttractorPositionParam = 0;
    a.AttractorEnabled = true; a.AttractorUseParamPosition = true; a.AttractorInfluenceFallOff = 5;
    a.AttractorPositionParamName = 7; a.AttractorInfluenceRadius = 10.0f; a.AttractorInfluenceForce = 2.0f;

    retail_std::pair<unsigned long, C3DVector> entries[2];
    entries[0].first = 3; entries[0].second = C3DVector(9.0f, 9.0f, 9.0f);
    entries[1].first = 7; entries[1].second = C3DVector(1.0f, 0.0f, 0.0f);
    CParticleUpdateParameters<unsigned long, unsigned long> params;
    params.VectorParams.Data._M_start = entries; params.VectorParams.Data._M_finish = entries + 2;
    bool changed = false;
    a.Update(sys, params, changed);
    // particle 0: delta = (-2,0,0), distance 2, within radius 10; acceleration reset then += (-1,0,0) * 2 * ((10-2)/10)^1
    if (!(Near(p[0].Acceleration.X, -1.6f) && Near(p[0].Acceleration.Y, 0.0f) && Near(p[0].Acceleration.Z, 0.0f))) ok = 0;
    if (!Near(p[0].ClosestAttractorDistance, 10.0f)) ok = 0;
    if (!Near(p[1].ClosestAttractorDistance, 100.0f) || g_currentPositionCalls != 0) ok = 0;   // out of range, map hit

    // closer attractor already applied: a larger radius must be skipped unless UseAllAttractors
    a.AttractorInfluenceRadius = 20.0f; a.AttractorPositionParamName = 99;   // no such key -> GetCurrentPosition
    g_currentPosition = C3DVector(1.0f, 0.0f, 0.0f);
    a.Update(sys, params, changed);
    if (g_currentPositionCalls != 1 || !Near(p[0].Acceleration.X, -1.6f)) ok = 0;
    sys.UseAllAttractors = true; a.AttractorInfluenceFallOff = 7;   // exponent 2, accumulate
    a.Update(sys, params, changed);
    // + (-1,0,0) * 2 * ((20-2)/20)^2 = -1.62
    if (!Near(p[0].Acceleration.X, -1.6f - 1.62f) || !Near(p[0].ClosestAttractorDistance, 20.0f)) ok = 0;
    a.AttractorInfluenceForce = 0.0f; a.Update(sys, params, changed);
    if (!Near(p[0].Acceleration.X, -1.6f - 1.62f)) ok = 0;   // zero force: nothing happens
    printf("00b179a0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}