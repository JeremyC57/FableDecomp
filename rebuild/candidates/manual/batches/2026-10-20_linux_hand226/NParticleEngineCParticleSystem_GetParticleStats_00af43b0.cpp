// NParticleEngine::CParticleSystem::GetParticleStats(CParticleStats& stats) -- add this system's component, particle and decal counts to the stats (active/allocated always, sleeping when the parent emitter sleeps, visible when the system is visible) and count its light components
// Leaf: GetParticleStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF43B0, 270 bytes, `ret 4`. PDB: ?GetParticleStats@CParticleSystem@NParticleEngine@@QAEXAAVCParticleStats@2@@Z.
// Callees: CPSCBase virtual slot 5 (+0x14, the component type; 5 = light component, descriptive).
// Layout: CParticleStats (Ego_r.pdb, 15 longs): ComponentsVisible +0x18, ComponentsActive +0x1C, ComponentsSleeping +0x20,
//   TotalNumParticlesVisible +0x24 / Allocated +0x28, TotalNumDecalsVisible +0x2C / Allocated +0x30,
//   TotalNumParticleLightsVisible +0x34 / Active +0x38. CParticleSystem with the retail container sizes (see 0x00AF4100):
//   PParentEmitter +4, Components +0x0C (24-byte entries), Particles/Decals STLport lists +0x1C/+0x20 (4-byte node pointer),
//   flag byte +0x50 (Visible = bit 5). CParticleEmitter: the PDB ushort bit-field word sits at retail +0x40 (12 bytes below
//   the PDB: STLport vector), Sleeping = bit 10.
// Shape: list sizes are the SGI `distance` walk (`++first` before `++n`); the emitter's Sleeping bit goes through an inline
//   `bool IsSleeping()` accessor (whole-word `xor ; mov dx ; shr ; test dl,1`), the system's own Visible bit is tested in place.
namespace NParticleEngine {

class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetType() const;                  // slot 5
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};

class CComponentInstance {                         // 0x14 bytes, vtable 0x0129F478
public:
    virtual ~CComponentInstance();
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
    virtual ~CParticleSystem();
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
