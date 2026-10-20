#include <cstdio>
namespace NParticleEngine {

class CPSCBase {
public:
    long type; const CPSCBase* got; float gotT;
    virtual long GetType() const { return type; }
    virtual void InterpolateTo(const CPSCBase& other, float t) { got = &other; gotT = t; }
    virtual ~CPSCBase() {}
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
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
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

class CParticleSystem {
public:
    virtual ~CParticleSystem() {}
    void*                   PParentEmitter;        // +0x04
    void*                   PEngine;               // +0x08
    vector<SComponentEntry> Components;            // +0x0c CVectorMap<EPSCType, CComponentInstance>
    bool                    ComponentsDirty;       // +0x18 (the map's comparator and Dirty bytes)
    void*                   Particles;             // +0x1c STLport list
    void*                   Decals;                // +0x20 STLport list
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
    float                   GlobalAlpha;           // +0x54
    float                   FadeAlpha;             // +0x58
    float                   NormalisedTime;        // +0x5c
    C3DVector               Scale;                 // +0x60
    void InterpolateTo(const CParticleSystem& other, float t);
};

void CParticleSystem::InterpolateTo(const CParticleSystem& other, float t)
{
    const SComponentEntry* theirs = other.Components.begin();
    for (SComponentEntry* it = Components.begin(); it != Components.end(); ++it, ++theirs) {
        CComponentInstance& mine = it->second;
        const CComponentInstance& their = theirs->second;
        if (mine.Component->GetType() == their.Component->GetType())
            mine.Component->InterpolateTo(*their.Component, t);
    }
    Enabled = (t > 0.5f ? other.Enabled : Enabled);
    Scale = Scale * (1.0f - t) + other.Scale * t;
    ScaleParticles = (t > 0.5f ? other.ScaleParticles : ScaleParticles);
}

} // namespace NParticleEngine

using namespace NParticleEngine;
static void Setup(CParticleSystem& s, SComponentEntry* e, CPSCBase* c, int n)
{
    for (int i = 0; i < n; ++i) { e[i].second.Component.Object = &c[i]; e[i].second.Component.Info = 0; c[i].got = 0; c[i].gotT = -1.0f; }
    s.Components._M_start = e; s.Components._M_finish = e + n; s.Components._M_end_of_storage = e + n;
}
int main()
{
    CPSCBase a[3], b[3]; SComponentEntry ea[3], eb[3];
    CParticleSystem s, o; Setup(s, ea, a, 3); Setup(o, eb, b, 3);
    a[0].type = 1; b[0].type = 1; a[1].type = 2; b[1].type = 3; a[2].type = 4; b[2].type = 4;
    s.Initialised = true; s.Enabled = false; s.Dead = true; s.NormalisedTimeEnabled = false; s.UseAllAttractors = true; s.Visible = false; s.StayWithEmitter = true; s.ScaleParticles = false;
    o.Initialised = false; o.Enabled = true; o.Dead = false; o.ScaleParticles = true; o.Visible = true;
    s.Scale.X = 1.0f; s.Scale.Y = 2.0f; s.Scale.Z = 4.0f; o.Scale.X = 3.0f; o.Scale.Y = 6.0f; o.Scale.Z = 8.0f;
    s.InterpolateTo(o, 0.25f);
    if (a[0].got != &b[0] || a[0].gotT != 0.25f || a[1].got != 0 || a[2].got != &b[2]) { std::printf("00af4100_TEST FAIL\n"); return 1; }
    if (s.Enabled != false || s.ScaleParticles != false || s.Initialised != true || s.Dead != true || s.Visible != false || s.StayWithEmitter != true || s.UseAllAttractors != true) { std::printf("00af4100_TEST FAIL\n"); return 1; }
    if (s.Scale.X != 1.5f || s.Scale.Y != 3.0f || s.Scale.Z != 5.0f) { std::printf("00af4100_TEST FAIL\n"); return 1; }
    s.InterpolateTo(o, 0.75f);
    if (s.Enabled != true || s.ScaleParticles != true || s.Initialised != true || s.Visible != false || s.Dead != true) { std::printf("00af4100_TEST FAIL\n"); return 1; }
    if (s.Scale.X != 1.5f * 0.25f + 3.0f * 0.75f || s.Scale.Z != 5.0f * 0.25f + 8.0f * 0.75f) { std::printf("00af4100_TEST FAIL\n"); return 1; }
    std::printf("00af4100_TEST PASS\n");
    return 0;
}