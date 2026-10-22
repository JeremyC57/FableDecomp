#include <stdio.h>
#include <string.h>
// NParticleEngine::CParticleSystem::Update(const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- per-frame update: reset the global alpha, take the emitter's update parameters, let every component contribute its parameters, sort both parameter maps, then update the enabled components; when none reports live content the system is marked dead and its particles and decals are destroyed
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
//   After the copy a named reference to UpdateParameters carries every later use (the CollectUpdateParameters / PreUpdate /
//   Update argument and both Sort calls), so its address stays in edi and params in ebx as retail.
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
    CUpdateParameters& parameters = UpdateParameters;
    SComponentEntry* it;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        instance.Component->CollectUpdateParameters(parameters, instance.InstanceName);
        instance.Component->UpdateStarted = false;
    }
    parameters.VectorParams.Sort();
    parameters.UpdateParams.Sort();
    bool alive = false;
    for (it = Components.begin(); it != Components.end(); ++it) {
        CComponentInstance& instance = it->second;
        if (instance.Enabled) {
            if (!instance.Component->UpdateStarted)
                instance.Component->PreUpdate(parameters);
            instance.Component->Update(*this, parameters, alive);
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

// ---- stubs and driver ----
using namespace NParticleEngine;
static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++fails; } }
static int g_vecAssign = 0, g_descAssign = 0, g_sortVec = 0, g_sortDesc = 0, g_deleted = 0, g_decals = 0;
template <> vector<pair<unsigned long, C3DVector> >& vector<pair<unsigned long, C3DVector> >::operator=(const vector& o)
{ ++g_vecAssign; _M_start = o._M_start; _M_finish = o._M_finish; _M_end_of_storage = o._M_end_of_storage; return *this; }
template <> vector<pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > >&
vector<pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > >::operator=(const vector& o)
{ ++g_descAssign; _M_start = o._M_start; _M_finish = o._M_finish; _M_end_of_storage = o._M_end_of_storage; return *this; }
typedef pair<unsigned long, C3DVector> TVecPair;
typedef pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > TDescPair;
typedef CUpdateParameters::CParticleUpdateCCompareVector<unsigned long> TVecCmp;
typedef CUpdateParameters::CParticleUpdateCCompareParamDesc<unsigned long> TDescCmp;
namespace NParticleEngine {
template <> void __fastcall sort<TVecPair, TVecCmp>(TVecPair*, TVecPair*, TVecCmp) { ++g_sortVec; }
template <> void __fastcall sort<TDescPair, TDescCmp>(TDescPair*, TDescPair*, TDescCmp) { ++g_sortDesc; }
}
template <> CUpdateParameters::~CParticleUpdateParameters() { }
CComponentInstance::~CComponentInstance() { }
CParticleSystem::~CParticleSystem() { }
void CParticleSystem::DeleteAllParticles() { ++g_deleted; }
void CParticleSystem::DestroyDecals() { ++g_decals; }
void CPSCBase::v00() {} void CPSCBase::v01() {} void CPSCBase::v02() {} void CPSCBase::v03() {} void CPSCBase::v04() {}
void CPSCBase::v05() {} void CPSCBase::v06() {} void CPSCBase::v07() {}
void CPSCBase::PreUpdate(const CUpdateParameters&) {}
void CPSCBase::Update(CParticleSystem&, const CUpdateParameters&, bool&) {}
void CPSCBase::CollectUpdateParameters(CUpdateParameters&, const unsigned long&) {}

class CTestComponent : public CPSCBase {
public:
    int collected, preUpdates, updates; unsigned long lastName; bool makeAlive; bool startOnCollect;
    const CUpdateParameters* seenParams; CParticleSystem* seenSystem; CPSCBase* startOther;
    virtual void PreUpdate(const CUpdateParameters& p) { ++preUpdates; seenParams = &p; if (startOther) startOther->UpdateStarted = true; }
    virtual void Update(CParticleSystem& s, const CUpdateParameters& p, bool& alive) { ++updates; seenSystem = &s; seenParams = &p; if (makeAlive) alive = true; }
    virtual void CollectUpdateParameters(CUpdateParameters& p, const unsigned long& name) { ++collected; lastName = name; seenParams = &p; UpdateStarted = true; }
};

int main()
{
    static unsigned char sysMem[sizeof(CParticleSystem)];
    CParticleSystem& sys = *(CParticleSystem*)sysMem;
    memset(sysMem, 0, sizeof sysMem);
    static CTestComponent compA, compB;
    CTestComponent* comps[2] = { &compA, &compB };
    for (int k = 0; k < 2; ++k) { CTestComponent* c = comps[k]; c->collected = c->preUpdates = c->updates = 0; c->makeAlive = false; c->startOther = 0; }
    static unsigned char entryMem[2][sizeof(SComponentEntry)];
    memset(entryMem, 0, sizeof entryMem);
    SComponentEntry* entries = (SComponentEntry*)entryMem;
    entries[0].second.Component.Object = &compA; entries[0].second.Enabled = true;  entries[0].second.InstanceName = 11;
    entries[1].second.Component.Object = &compB; entries[1].second.Enabled = false; entries[1].second.InstanceName = 22;
    sys.Components._M_start = entries; sys.Components._M_finish = entries + 2;
    static unsigned char paramMem[sizeof(CUpdateParameters)];
    memset(paramMem, 0, sizeof paramMem);
    CUpdateParameters& params = *(CUpdateParameters*)paramMem;
    params.VectorParams.Dirty = true; params.UpdateParams.Dirty = false;
    sys.GlobalAlpha = 0.25f;

    // pass 1: A enabled and alive; B disabled. The collect loop clears UpdateStarted, so A gets PreUpdate.
    compA.makeAlive = true;
    sys.Update(params);
    check(sys.GlobalAlpha == 1.0f, "alpha reset");
    check(g_vecAssign == 1 && g_descAssign == 1, "both maps copied");
    check(sys.UpdateParameters.UpdateParams.Dirty == false, "clean flag copied");
    check(compA.collected == 1 && compB.collected == 1 && compA.lastName == 11 && compB.lastName == 22, "every component collects");
    check(compA.seenParams == &sys.UpdateParameters, "collect gets the system parameters");
    check(g_sortVec == 1 && g_sortDesc == 0, "only the dirty map sorted");
    check(sys.UpdateParameters.VectorParams.Dirty == false, "sorted map clean");
    check(compA.preUpdates == 1 && compB.preUpdates == 0, "not-started enabled component gets PreUpdate");
    check(compA.updates == 1 && compB.updates == 0 && compA.seenSystem == &sys, "only enabled updated");
    check(g_deleted == 0 && g_decals == 0, "alive keeps particles");

    // pass 2: A's collect leaves it started... force not started via a component that clears it: use B enabled, not alive
    entries[1].second.Enabled = true; compA.makeAlive = false; compA.startOther = &compB;   // A starts B before B is reached
    params.UpdateParams.Dirty = true;
    sys.Initialised = true; sys.Dead = false;
    sys.Update(params);
    check(g_sortDesc == 1, "desc map sorted when dirty");
    check(compB.updates == 1 && compB.preUpdates == 0 && compA.preUpdates == 2, "started component skips PreUpdate");
    check(g_deleted == 1 && g_decals == 1 && sys.Dead && !sys.Initialised, "nothing alive marks the system dead");

    // pass 3: empty component list
    sys.Components._M_finish = entries; sys.Dead = false; sys.Initialised = true;
    sys.Update(params);
    check(g_deleted == 2 && sys.Dead, "empty list is dead");

    if (fails) { puts("00af59a0_TEST FAIL"); return 1; }
    puts("00af59a0_TEST PASS");
    return 0;
}