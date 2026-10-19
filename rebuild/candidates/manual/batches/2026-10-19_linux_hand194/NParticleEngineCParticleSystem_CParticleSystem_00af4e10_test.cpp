// Behaviour test for NParticleEngine::CParticleSystem::CParticleSystem (0x00AF4E10): empty containers, self-linked list
// sentinels, flag byte and unit alpha/scale, built over garbage-filled storage.
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
struct CBaseClass {
    virtual ~CBaseClass() {}
    CBaseClass() {}
};

struct CCharString {
    void* m_pStringData;
    CCharString() : m_pStringData(0) {}
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace NParticleEngine {

struct SEmptyCompare { };

template <class T> struct SRetailAllocProxy {     // STLport _STLP_alloc_proxy<T*, T, allocator<T>>: end of storage
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) { }
};

template <class T> struct CRetailVectorBase {     // STLport _Vector_base<T>
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVectorBase() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

template <class T> struct CRetailVectorMap : CRetailVectorBase<T> {   // CVectorMap<K, V, Compare>
    SEmptyCompare m_compare;
    bool Dirty;
    CRetailVectorMap() : Dirty(false) { }
};

template <class T> struct SListNode { SListNode* m_pNext; SListNode* m_pPrev; T m_value; };
template <class T> struct CRetailList {            // std::list<T> (retail: sentinel pointer only)
    SListNode<T>* m_pHead;
    CRetailList()
    {
        m_pHead = 0;
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct SComponentEntry { long m_type; void* m_instance; };
struct SVectorParam { unsigned long m_key; float m_value[3]; };
struct SParamDesc { unsigned long m_key; unsigned long m_desc[3]; };
struct CParticle;
struct CDecal;
struct CParticleEmitter;
struct CIEngine;

struct CParticleUpdateParameters : CBaseClass {    // vtable 0x0125FFDC
    CRetailVectorMap<SVectorParam> VectorParams;   // +0x04
    CRetailVectorMap<SParamDesc> UpdateParams;     // +0x14
};

struct CParticleSystem : CBaseClass {              // vtable 0x0129F4E8
    CParticleEmitter* PParentEmitter;              // +0x04
    CIEngine* PEngine;                             // +0x08
    CRetailVectorMap<SComponentEntry> Components;  // +0x0C
    CRetailList<CParticle*> Particles;             // +0x1C
    CRetailList<CDecal*> Decals;                   // +0x20
    CParticleUpdateParameters UpdateParameters;    // +0x24
    long NoActiveParticles;                        // +0x48
    long NoActiveDecals;                           // +0x4C
    bool Initialised : 1;                          // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                             // +0x54
    float FadeAlpha;                               // +0x58
    float NormalisedTime;                          // +0x5C
    C3DVector Scale;                               // +0x60
    CCharString Name;                              // +0x6C
    CParticleSystem();
};

CParticleSystem::CParticleSystem()
    : PEngine(0)
{
    GlobalAlpha = 1.0f;
    FadeAlpha = 1.0f;
    PParentEmitter = 0;
    NoActiveParticles = 0;
    NoActiveDecals = 0;
    NormalisedTime = 0.0f;
    Initialised = false;
    Enabled = true;
    Dead = false;
    NormalisedTimeEnabled = false;
    UseAllAttractors = false;
    Visible = false;
    StayWithEmitter = false;
    ScaleParticles = true;
    Scale = C3DVector(1.0f, 1.0f, 1.0f);
}

}   // namespace NParticleEngine

int main()
{
    using namespace NParticleEngine;
    void* mem = malloc(sizeof(CParticleSystem)); memset(mem, 0xcd, sizeof(CParticleSystem));
    CParticleSystem* p = new (mem) CParticleSystem;
    int bad = 0;
    if (p->PParentEmitter || p->PEngine || p->Components._M_start || p->Components._M_finish || p->Components._M_end_of_storage._M_data || p->Components.Dirty) bad = 1;
    if (!p->Particles.m_pHead || p->Particles.m_pHead->m_pNext != p->Particles.m_pHead || p->Particles.m_pHead->m_pPrev != p->Particles.m_pHead) bad = 2;
    if (!p->Decals.m_pHead || p->Decals.m_pHead->m_pNext != p->Decals.m_pHead || p->Decals.m_pHead == (void*)p->Particles.m_pHead) bad = 3;
    if (p->UpdateParameters.VectorParams._M_start || p->UpdateParameters.UpdateParams._M_finish || p->UpdateParameters.UpdateParams.Dirty) bad = 4;
    if (p->NoActiveParticles || p->NoActiveDecals || p->NormalisedTime != 0.0f) bad = 5;
    if (p->Initialised || !p->Enabled || p->Dead || p->Visible || !p->ScaleParticles || p->UseAllAttractors) bad = 6;
    if (p->GlobalAlpha != 1.0f || p->FadeAlpha != 1.0f || p->Scale.X != 1.0f || p->Scale.Y != 1.0f || p->Scale.Z != 1.0f) bad = 7;
    if (bad) { std::printf("00af4e10_TEST FAIL\n"); return 1; }
    std::printf("00af4e10_TEST PASS\n");
    return 0;
}
