#include <cstdio>
namespace NParticleEngine {

class CPSCBase {
public:
    long type;
    virtual long GetType() const { return type; }
    virtual ~CPSCBase() {}
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};

class CComponentInstance {                         // 0x14 bytes, vtable 0x0129F478
public:
    virtual ~CComponentInstance() {}
    CCountedPointer<CPSCBase> Component;           // +0x04
    bool                      Enabled;             // +0x0c
    unsigned long             Flags;               // +0x10
};

struct SComponentEntry {                           // pair<EPSCType, CComponentInstance>, 0x18 bytes
    unsigned long      first;
    CComponentInstance second;
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    long size() const { return _M_finish - _M_start; }
};

template <class T> struct list {                   // STLport list: the 4-byte node pointer
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    _Node* _M_node;
    long size() const
    {
        long n = 0;
        for (_Node* first = _M_node->_M_next; first != _M_node; ) {
            first = first->_M_next;
            ++n;
        }
        return n;
    }
};

class CParticleStats {
public:
    long EmittersVisible, EmittersActive, EmittersSleeping;
    long SystemsVisible, SystemsActive, SystemsSleeping;
    long ComponentsVisible, ComponentsActive, ComponentsSleeping;
    long TotalNumParticlesVisible, TotalNumParticlesAllocated;
    long TotalNumDecalsVisible, TotalNumDecalsAllocated;
    long TotalNumParticleLightsVisible, TotalNumParticleLightsActive;
};

class CParticle;
class CDecal;

class CParticleEmitter {
public:
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;                   // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    bool IsSleeping() const { return Sleeping; }
};

class CParticleSystem {
public:
    virtual ~CParticleSystem() {}
    CParticleEmitter*       PParentEmitter;        // +0x04
    void*                   PEngine;               // +0x08
    vector<SComponentEntry> Components;            // +0x0c CVectorMap<EPSCType, CComponentInstance>
    bool                    ComponentsDirty;       // +0x18
    list<CParticle*>        Particles;             // +0x1c
    list<CDecal*>           Decals;                // +0x20
    unsigned char           UpdateParameters[0x24];// +0x24
    long                    NoActiveParticles;     // +0x48
    long                    NoActiveDecals;        // +0x4c
    bool                    Initialised : 1;       // +0x50
    bool                    Enabled : 1;
    bool                    Dead : 1;
    bool                    NormalisedTimeEnabled : 1;
    bool                    UseAllAttractors : 1;
    bool                    Visible : 1;
    bool                    StayWithEmitter : 1;
    bool                    ScaleParticles : 1;
    void GetParticleStats(CParticleStats& stats);
};

void CParticleSystem::GetParticleStats(CParticleStats& stats)
{
    stats.ComponentsActive += Components.size();
    stats.TotalNumParticlesAllocated += Particles.size();
    stats.TotalNumDecalsAllocated += Decals.size();
    if (PParentEmitter->IsSleeping())
        stats.ComponentsSleeping += Components.size();
    else if (Visible) {
        stats.ComponentsVisible += Components.size();
        stats.TotalNumParticlesVisible += Particles.size();
        stats.TotalNumDecalsVisible += Decals.size();
    }
    for (SComponentEntry* it = Components.begin(); it != Components.end(); ++it) {
        if (it->second.Component->GetType() == 5) {
            ++stats.TotalNumParticleLightsActive;
            if (Visible)
                ++stats.TotalNumParticleLightsVisible;
        }
    }
}

} // namespace NParticleEngine

using namespace NParticleEngine;
template <class T> static void MakeList(list<T>& l, typename list<T>::_Node* nodes, int n)
{
    typename list<T>::_Node* head = &nodes[n];
    typename list<T>::_Node* prev = head;
    for (int i = 0; i < n; ++i) { prev->_M_next = &nodes[i]; prev = &nodes[i]; }
    prev->_M_next = head;
    l._M_node = head;
}
int main()
{
    CPSCBase c[3]; c[0].type = 5; c[1].type = 2; c[2].type = 5;
    SComponentEntry e[3];
    for (int i = 0; i < 3; ++i) { e[i].second.Component.Object = &c[i]; e[i].second.Component.Info = 0; }
    CParticleSystem s; s.Components._M_start = e; s.Components._M_finish = e + 3; s.Components._M_end_of_storage = e + 3;
    list<CParticle*>::_Node pn[5]; MakeList(s.Particles, pn, 4);
    list<CDecal*>::_Node dn[3]; MakeList(s.Decals, dn, 2);
    CParticleEmitter em; em.Sleeping = 0; em.Priority = 7; em.WaitingToSleep = 1; em.Emitter2D = 1;
    s.PParentEmitter = &em; s.Visible = true;
    CParticleStats st = { 0 };
    s.GetParticleStats(st);
    if (st.ComponentsActive != 3 || st.TotalNumParticlesAllocated != 4 || st.TotalNumDecalsAllocated != 2 || st.ComponentsSleeping != 0 ||
        st.ComponentsVisible != 3 || st.TotalNumParticlesVisible != 4 || st.TotalNumDecalsVisible != 2 ||
        st.TotalNumParticleLightsActive != 2 || st.TotalNumParticleLightsVisible != 2) { std::printf("00af43b0_TEST FAIL\n"); return 1; }
    em.Sleeping = 1;
    s.GetParticleStats(st);
    if (st.ComponentsActive != 6 || st.ComponentsSleeping != 3 || st.ComponentsVisible != 3 || st.TotalNumParticlesVisible != 4 ||
        st.TotalNumParticleLightsActive != 4 || st.TotalNumParticleLightsVisible != 4) { std::printf("00af43b0_TEST FAIL\n"); return 1; }
    em.Sleeping = 0; s.Visible = false;
    s.GetParticleStats(st);
    if (st.ComponentsActive != 9 || st.ComponentsSleeping != 3 || st.ComponentsVisible != 3 || st.TotalNumDecalsAllocated != 6 ||
        st.TotalNumParticleLightsActive != 6 || st.TotalNumParticleLightsVisible != 4) { std::printf("00af43b0_TEST FAIL\n"); return 1; }
    std::printf("00af43b0_TEST PASS\n");
    return 0;
}
