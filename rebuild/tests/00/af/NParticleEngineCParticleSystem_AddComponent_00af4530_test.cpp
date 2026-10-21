// Behaviour test for 00af4530: src/00af4530.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NParticleEngine::CParticleSystem::AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool unused)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CParticleSystem::AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool unused) -- create a component of the named type through the component factory, attach it to this system, wrap it in a component instance and insert it into the component map at the upper bound of its type
// Leaf: AddComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4530, 298 bytes, `ret 10h`. PDB: ?AddComponent@CParticleSystem@NParticleEngine@@QAEAAVCPSCBase@2@ABVCCharString@@ABK_N2@Z
//   (returns the component by reference; the fourth argument is not read by the retail body).
// Callees: the component factory getter 0x00AEAB00 (returns the global 0x013D2E24; catalogued "GFGetPreMainMemoryUsed") and
//   its create-by-name 0x00AF28C0 (descriptive names), CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0,
//   CCountedPointer<CPSCBase>::Reset(CPSCBase*) 0x00AF6010, CVectorMap<EPSCType, CComponentInstance>::UpperBound 0x00AF6120,
//   vector<pair<EPSCType, CComponentInstance> >::insert(iterator, const value_type&) 0x00AF7730, operator delete 0x00BFE9BC;
//   CPSCBase virtual slot 4 (+0x10, the component type).
// Layout: CComponentInstance (0x14, vtable 0x0129F478) = CBaseClass vptr, CCountedPointer<CPSCBase> Component +4
//   {Object, Info}, Enabled +0x0C (defaults true), InstanceName +0x10; CPSCBase.ParticleSystem +4; Components +0x0C.
// Shape: the instance is default-constructed then assigned (zero stores, then the out-of-line Reset); the map entry
//   `value_type(type, instance)` is the right-most argument of insert and is built before the UpperBound call (`type` is a
//   named local homed in the dead `enabled` slot); both the entry and the instance are destroyed inline (counted release
//   re-reading the block after the decrement, both words zeroed, then ~CBaseClass).
namespace NParticleEngine {

class CParticleSystem;
class CCharString { public: void* PData; };

class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long GetType() const;                  // slot 4
    CParticleSystem* ParticleSystem;               // +0x04
};

class CPSCFactory {
public:
    static CPSCFactory* Get();                     // 0x00AEAB00
    CPSCBase* CreateComponent(const CCharString& typeName);   // 0x00AF28C0
};

struct SCountBlock {
    long  Count;                                   // +0x00
    void (__fastcall* Destroy)(void* object);      // +0x04
    void* Object;                                  // +0x08
};

template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Info;
    CCountedPointer() : Object(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Info(other.Info)
    {
        if (Info)
            ++Info->Count;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Object = 0;
        Info = 0;
    }
    void Reset(T* object);                         // 0x00AF6010 for T = CPSCBase
};

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};

class CComponentInstance : public CBaseClass {     // 0x14 bytes, vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;           // +0x04
    bool                      Enabled;             // +0x0c
    unsigned long             InstanceName;        // +0x10
    CComponentInstance() : Enabled(true) {}
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
typedef pair<long, CComponentInstance> SComponentEntry;

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* insert(T* position, const T& x);            // 0x00AF7730 for this T
};

template <class K, class V> struct CVectorMap {    // 0x10 bytes
    vector<pair<K, V> > Vector;
    unsigned char       Compare;
    bool                Dirty;
    pair<K, V>* UpperBound(const K& key);          // 0x00AF6120 for this map
};

#pragma auto_inline(off)
template <class K, class V> __declspec(noinline) pair<K, V>* CVectorMap<K, V>::UpperBound(const K& key)
{
    pair<K, V>* first = Vector._M_start;
    long count = Vector._M_finish - first;
    while (count > 0) {
        long half = count / 2;
        if (!(key < first[half].first)) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
#pragma auto_inline(on)
class CParticleSystem {
public:
    virtual ~CParticleSystem();
    void*                                  PParentEmitter;   // +0x04
    void*                                  PEngine;          // +0x08
    CVectorMap<long, CComponentInstance>   Components;       // +0x0c
    CPSCBase& AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool unused);
};

CPSCBase& CParticleSystem::AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool)
{
    CPSCBase* component = CPSCFactory::Get()->CreateComponent(typeName);
    component->ParticleSystem = this;
    CComponentInstance instance;
    instance.Component.Reset(component);
    instance.Enabled = enabled;
    instance.InstanceName = instanceName;
    long type = component->GetType();
    Components.Vector.insert(Components.UpperBound(type), SComponentEntry(type, instance));
    return *component;
}

} // namespace NParticleEngine

// ---- stubs and driver ----
void NParticleEngine::CPSCBase::v00() {}
void NParticleEngine::CPSCBase::v01() {}
void NParticleEngine::CPSCBase::v02() {}
void NParticleEngine::CPSCBase::v03() {}
#include <stdlib.h>
namespace NParticleEngine {
struct CComp : CPSCBase { long type; long GetType() const { return type; } };
long CPSCBase::GetType() const { return 0; }
static CPSCFactory g_factory; static const CCharString* g_typeName; static CComp g_comps[4]; static int g_made = 0;
CPSCFactory* CPSCFactory::Get() { return &g_factory; }
CPSCBase* CPSCFactory::CreateComponent(const CCharString& typeName) { g_typeName = &typeName; CComp* c = &g_comps[g_made++]; c->type = (long)typeName.PData; return c; }
static int g_destroyed = 0;
static void __fastcall DestroyComp(void*) { ++g_destroyed; }
template <> void CCountedPointer<CPSCBase>::Reset(CPSCBase* object)
{
    Object = object; Info = (SCountBlock*)malloc(sizeof(SCountBlock)); Info->Count = 1; Info->Destroy = DestroyComp; Info->Object = object;
}
static int g_bases = 0, g_baseDtors = 0;
CBaseClass::CBaseClass() { ++g_bases; }
CBaseClass::~CBaseClass() { ++g_baseDtors; }
static SComponentEntry* g_insPos; static long g_insType; static bool g_insEnabled; static unsigned long g_insName; static CPSCBase* g_insComp;
template <> SComponentEntry* vector<SComponentEntry>::insert(SComponentEntry* position, const SComponentEntry& x)
{
    g_insPos = position; g_insType = x.first; g_insEnabled = x.second.Enabled; g_insName = x.second.InstanceName; g_insComp = x.second.Component.Object;
    CHECK(x.second.Component.Info->Count == 2);       // the entry's copy and the instance
    ++x.second.Component.Info->Count;                  // the stored copy keeps the component alive
    return position;
}
CParticleSystem::~CParticleSystem() {}
}
using namespace NParticleEngine;
int main()
{
    static unsigned char store[sizeof(CParticleSystem)]; CParticleSystem* ps = (CParticleSystem*)store; memset(store, 0, sizeof store);
    static unsigned char es[3][sizeof(SComponentEntry)]; SComponentEntry* e = (SComponentEntry*)es;
    e[0].first = 1; e[1].first = 3; e[2].first = 5;
    ps->Components.Vector._M_start = e; ps->Components.Vector._M_finish = e + 3; ps->Components.Vector._M_end_of_storage = e + 3;
    CCharString name; name.PData = (void*)3; unsigned long inst = 0x99;
    CPSCBase& r = ps->AddComponent(name, inst, false, true);
    CHECK(&r == &g_comps[0]); CHECK(r.ParticleSystem == ps); CHECK(g_typeName == &name);
    CHECK(g_insPos == e + 2); CHECK(g_insType == 3); CHECK(g_insEnabled == false); CHECK(g_insName == 0x99); CHECK(g_insComp == &g_comps[0]);
    CHECK(g_destroyed == 0); CHECK(g_bases == 1);
    name.PData = (void*)0; ps->AddComponent(name, inst, true, false); CHECK(g_insPos == e); CHECK(g_insEnabled == true); CHECK(g_destroyed == 0); CHECK(g_baseDtors == 4);
    if (fails == 0) printf("00af4530_TEST PASS\n"); else printf("00af4530_TEST FAIL\n");
    return fails;
}