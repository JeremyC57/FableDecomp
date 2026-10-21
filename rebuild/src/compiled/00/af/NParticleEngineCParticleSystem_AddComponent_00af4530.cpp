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