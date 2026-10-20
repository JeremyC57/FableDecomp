// NParticleEngine::CParticleSystem::Update(const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- per-frame update: reset the global alpha, take the emitter's update parameters, let every component contribute its parameters, sort both parameter maps, then update the enabled components; when none reports live content the system is marked dead and its particles and decals are destroyed
// Grade: DIFFER(269v280) -- register colouring only: retail params ebx / &UpdateParameters edi / iterator ebx / instance ebp, ours params edi / &UpdateParameters ebx / iterator ebp / instance edi; the second loop head then loses retail's 7-byte alignment lea (11 bytes)
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF59A0, 280 bytes, `ret 4`. PDB: ?Update@CParticleSystem@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@@Z.
// Callees: vector<pair<unsigned long, C3DVector> >::operator= 0x006E2000 and vector<pair<unsigned long,
//   CParticleUpdateParamDesc> >::operator= 0x006E22F0 (both catalogued as propagated std/CCombatWheel names), SGI
//   sort<...>(first, last, comp) 0x0076C000 / 0x0076C1A0 (`__fastcall`, comparator by value), CParticleSystem::
//   DeleteAllParticles 0x00AF56E0 and DestroyDecals 0x00AF5770; CPSCBase virtual slots 8 (+0x20, one-argument pre-update,
//   descriptive name), 9 (+0x24) Update(CParticleSystem&, const CParticleUpdateParameters&, bool&) and 10 (+0x28)
//   CollectUpdateParameters(CParticleUpdateParameters&, const unsigned long&) (PDB, CPSCUpdateNormal overrides).
// Layout: CParticleUpdateParameters<ulong, ulong> (retail 0x24: vptr, VectorParams +4, UpdateParams +0x14, each a 0x10-byte
//   CVectorMap {12-byte STLport vector, empty comparator +0x0C, Dirty +0x0D}); CPSCBase {vptr, ParticleSystem +4,
//   UpdateStarted:1 / Initialised:1 +8}; CComponentInstance InstanceName +0x10; CParticleSystem as in 0x00AF4100
//   (UpdateParameters +0x24, flag byte +0x50, GlobalAlpha +0x54).
// Shape: the parameters are copied by the implicit operator= of the polymorphic CParticleUpdateParameters (no vptr copy:
//   vector operator= out of line, then the Dirty byte); CVectorMap::Sort is inline (`if (Dirty) { sort(...); Dirty = false; }`);
//   per-iteration instance references give the second induction pointer; `alive` is a frame byte passed by reference.
namespace NParticleEngine {

class CParticleSystem;
struct C3DVector { float X, Y, Z; };
template <class A, class B> class CParticleUpdateParamDesc { public: unsigned long _body[3]; };

template <class K, class V> struct pair { K first; V second; };

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    vector& operator=(const vector& other);        // 0x006E2000 / 0x006E22F0
};

template <class T, class Compare> void __fastcall sort(T* first, T* last, Compare comp);   // 0x0076C000 / 0x0076C1A0

template <class K, class V, class C> class CVectorMap {
public:
    vector<pair<K, V> > Vector;                    // +0x00
    C                   Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    void Sort()
    {
        if (Dirty) {
            sort(Vector.begin(), Vector.end(), Compare);
            Dirty = false;
        }
    }
};

template <class A, class B> class CParticleUpdateParameters {   // retail 0x24 bytes
public:
    template <class K> struct CParticleUpdateCCompareVector {};
    template <class K> struct CParticleUpdateCCompareParamDesc {};
    virtual ~CParticleUpdateParameters();
    CVectorMap<unsigned long, C3DVector, CParticleUpdateCCompareVector<unsigned long> > VectorParams;                       // +0x04
    CVectorMap<unsigned long, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<unsigned long> > UpdateParams; // +0x14
};
typedef CParticleUpdateParameters<unsigned long, unsigned long> CUpdateParameters;

class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void PreUpdate(const CUpdateParameters& params);                                         // slot 8
    virtual void Update(CParticleSystem& system, const CUpdateParameters& params, bool& alive);      // slot 9
    virtual void CollectUpdateParameters(CUpdateParameters& params, const unsigned long& name);      // slot 10
    CParticleSystem* ParticleSystem;               // +0x04
    bool             UpdateStarted : 1;            // +0x08
    bool             Initialised : 1;
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
    unsigned long             InstanceName;        // +0x10
};

struct SComponentEntry {                           // pair<EPSCType, CComponentInstance>, 0x18 bytes
    unsigned long      first;
    CComponentInstance second;
};

class CParticleSystem {
public:
    virtual ~CParticleSystem();
    void*                    PParentEmitter;       // +0x04
    void*                    PEngine;              // +0x08
    vector<SComponentEntry>  Components;           // +0x0c CVectorMap<EPSCType, CComponentInstance>
    bool                     ComponentsDirty;      // +0x18
    void*                    Particles;            // +0x1c STLport list
    void*                    Decals;               // +0x20 STLport list
    CUpdateParameters        UpdateParameters;     // +0x24
    long                     NoActiveParticles;    // +0x48
    long                     NoActiveDecals;       // +0x4c
    bool                     Initialised : 1;      // +0x50
    bool                     Enabled : 1;
    bool                     Dead : 1;
    bool                     NormalisedTimeEnabled : 1;
    bool                     UseAllAttractors : 1;
    bool                     Visible : 1;
    bool                     StayWithEmitter : 1;
    bool                     ScaleParticles : 1;
    float                    GlobalAlpha;          // +0x54
    void DeleteAllParticles();                     // 0x00AF56E0
    void DestroyDecals();                          // 0x00AF5770
    void Update(const CUpdateParameters& params);
};

void CParticleSystem::Update(const CUpdateParameters& params)
{
    GlobalAlpha = 1.0f;
    UpdateParameters = params;
    SComponentEntry* it;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        instance.Component->CollectUpdateParameters(UpdateParameters, instance.InstanceName);
        instance.Component->UpdateStarted = false;
    }
    UpdateParameters.VectorParams.Sort();
    UpdateParameters.UpdateParams.Sort();
    bool alive = false;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        if (instance.Enabled) {
            if (!instance.Component->UpdateStarted)
                instance.Component->PreUpdate(UpdateParameters);
            instance.Component->Update(*this, UpdateParameters, alive);
        }
    }
    if (!alive) {
        Initialised = false;
        DeleteAllParticles();
        DestroyDecals();
        Dead = true;
    }
}

} // namespace NParticleEngine
