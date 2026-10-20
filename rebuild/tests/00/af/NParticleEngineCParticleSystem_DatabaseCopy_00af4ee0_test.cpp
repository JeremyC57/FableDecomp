#include <cstdio>
static int g_info; static unsigned long g_resized;
namespace NParticleEngine {

class CParticleSystem;

class CCharString {
public:
    void* PData;
    CCharString& operator=(const CCharString& other) { PData = other.PData; return *this; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CPSCBase {
public:
    long type; const CPSCBase* copiedFrom; int clones;
    virtual CPSCBase* DatabaseClone() const;
    virtual long GetType() const { return type; }
    virtual void DatabaseCopy(const CPSCBase& other) { copiedFrom = &other; }
    virtual ~CPSCBase() {}
    CParticleSystem* ParticleSystem;               // +0x04
    bool IsSameType(const CPSCBase& other) const { return GetType() == other.GetType(); }
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
    void Reset(T* object) { Object = object; Info = &g_info; }
};

class CComponentInstance {                         // 0x14 bytes, vtable 0x0129F478
public:
    virtual ~CComponentInstance() {}
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
    void resize(unsigned long n) { g_resized = n; _M_finish = _M_start + n; }
};

class CParticleSystem {
public:
    virtual ~CParticleSystem() {}
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

namespace NParticleEngine {
static CPSCBase g_pool[4]; static int g_poolUsed;
CPSCBase* CPSCBase::DatabaseClone() const { CPSCBase* c = &g_pool[g_poolUsed++]; c->type = type; c->copiedFrom = 0; c->clones = 0; c->ParticleSystem = 0; ++const_cast<CPSCBase*>(this)->clones; return c; }
}
using namespace NParticleEngine;
int main()
{
    CPSCBase src[3], dstc[2];
    for (int i = 0; i < 3; ++i) { src[i].type = 10 + i; src[i].clones = 0; src[i].copiedFrom = 0; }
    dstc[0].type = 10; dstc[0].copiedFrom = 0; dstc[1].type = 99; dstc[1].copiedFrom = 0;
    SComponentEntry se[3], de[3];
    for (int i = 0; i < 3; ++i) { se[i].first = 100 + i; se[i].second.Component.Object = &src[i]; se[i].second.InstanceName = 200 + i; }
    de[0].second.Component.Object = &dstc[0]; de[1].second.Component.Object = &dstc[1]; de[2].second.Component.Object = 0;
    for (int i = 0; i < 3; ++i) { de[i].first = 0; de[i].second.InstanceName = 0; de[i].second.Component.Info = 0; }
    CParticleSystem o, s;
    o.Components._M_start = se; o.Components._M_finish = se + 3; o.Components._M_end_of_storage = se + 3;
    s.Components._M_start = de; s.Components._M_finish = de + 1; s.Components._M_end_of_storage = de + 3;
    int nameTag; o.Name.PData = &nameTag; s.Name.PData = 0;
    o.Enabled = true; o.ScaleParticles = false; o.Visible = false; s.Enabled = false; s.ScaleParticles = true; s.Visible = true; s.Dead = true;
    o.Scale.X = 1; o.Scale.Y = 2; o.Scale.Z = 3; s.Scale.X = s.Scale.Y = s.Scale.Z = 0;
    s.DatabaseCopy(o);
    if (g_resized != 3 || s.Name.PData != &nameTag || de[0].first != 100 || de[2].first != 102 || de[1].second.InstanceName != 201) { std::printf("00af4ee0_TEST FAIL\n"); return 1; }
    if (de[0].second.Component.Object != &dstc[0] || dstc[0].copiedFrom != &src[0] || src[0].clones != 0) { std::printf("00af4ee0_TEST FAIL\n"); return 1; }
    if (de[1].second.Component.Object == &dstc[1] || src[1].clones != 1 || de[1].second.Component->type != 11 || de[1].second.Component->ParticleSystem != &s) { std::printf("00af4ee0_TEST FAIL\n"); return 1; }
    if (src[2].clones != 1 || de[2].second.Component->ParticleSystem != &s) { std::printf("00af4ee0_TEST FAIL\n"); return 1; }
    if (s.Enabled != true || s.ScaleParticles != false || s.Visible != true || s.Dead != true || s.Scale.Y != 2 || s.Scale.Z != 3) { std::printf("00af4ee0_TEST FAIL\n"); return 1; }
    std::printf("00af4ee0_TEST PASS\n");
    return 0;
}