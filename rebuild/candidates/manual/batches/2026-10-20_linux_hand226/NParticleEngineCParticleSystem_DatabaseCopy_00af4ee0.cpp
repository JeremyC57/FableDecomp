// NParticleEngine::CParticleSystem::DatabaseCopy(const CParticleSystem& other) -- copy a database system into this one: the name, then the component list entry by entry (a component of the same type copies in place, otherwise it is replaced by a clone owned by this system), then the Enabled flag, Scale and the ScaleParticles flag
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4EE0, 299 bytes, `ret 4`. PDB: ?DatabaseCopy@CParticleSystem@NParticleEngine@@QAEXABV12@@Z.
// Callees: CCharString::operator= 0x0099EFB0, vector<pair<EPSCType, CComponentInstance> >::resize(n) 0x00AF7A10 (catalogued
//   "CThingCreatureBase::GetAnimsWithNameStartingWith", propagated), CCountedPointer<CPSCBase>::Reset(CPSCBase*) 0x00AF6010
//   (catalogued for CIPhysicsFacesSegmentBase, propagated); CPSCBase virtual slots 2 (+0x08) DatabaseClone, 5 (+0x14) the
//   component type (descriptive), 18 (+0x48) DatabaseCopy(const CPSCBase&) (PDB names from the CPSC* overrides).
// Layout: CParticleSystem as in 0x00AF4100 (Components +0x0C, flag byte +0x50 with Enabled bit 1 and ScaleParticles bit 7,
//   Scale +0x60, Name +0x6C); CPSCBase.ParticleSystem +4; CComponentInstance InstanceName +0x10.
// Shape: entries reached through STLport `operator[]` = `*(begin() + n)`; the counted-pointer replacement is out of line;
//   the type comparison is an inline bool helper on the component (`IsSameType`, descriptive): written as `a == b` in the
//   `&&` chain VC evaluates the right call first, in the helper it calls `this` first and compares `cmp ecx,eax` like retail;
//   C3DVector's user copy ctor next to the implicit operator= gives the `lea` pointer copy of Scale.
namespace NParticleEngine {

class CParticleSystem;

class CCharString {
public:
    void* PData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CPSCBase {
public:
    virtual void v00(); virtual void v01();
    virtual CPSCBase* DatabaseClone() const;                         // slot 2
    virtual void v03(); virtual void v04();
    virtual long GetType() const;                                    // slot 5
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void DatabaseCopy(const CPSCBase& other);                // slot 18
    CParticleSystem* ParticleSystem;               // +0x04
    bool IsSameType(const CPSCBase& other) const { return GetType() == other.GetType(); }
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
    void Reset(T* object);                         // 0x00AF6010 for T = CPSCBase
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

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
    void resize(unsigned long n);                  // 0x00AF7A10 for this T
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
    unsigned char            UpdateParameters[0x24];// +0x24
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
    float                    FadeAlpha;            // +0x58
    float                    NormalisedTime;       // +0x5c
    C3DVector                Scale;                // +0x60
    CCharString              Name;                 // +0x6c
    void DatabaseCopy(const CParticleSystem& other);
};

void CParticleSystem::DatabaseCopy(const CParticleSystem& other)
{
    Name = other.Name;
    unsigned long count = other.Components.size();
    Components.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        Components[i].first = other.Components[i].first;
        CComponentInstance& mine = Components[i].second;
        const CComponentInstance& theirs = other.Components[i].second;
        if (mine.Component.Object != 0 && mine.Component->IsSameType(*theirs.Component))
            mine.Component->DatabaseCopy(*theirs.Component);
        else {
            mine.Component.Reset(theirs.Component->DatabaseClone());
            mine.Component->ParticleSystem = this;
        }
        mine.InstanceName = theirs.InstanceName;
    }
    Enabled = other.Enabled;
    Scale = other.Scale;
    ScaleParticles = other.ScaleParticles;
}

} // namespace NParticleEngine
