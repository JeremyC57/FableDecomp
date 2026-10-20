// NParticleEngine::CParticleSystem::InterpolateTo(const CParticleSystem& other, float t) -- interpolate towards another system: every component pair of the same type interpolates, the Enabled and ScaleParticles flags switch to the other system's past the halfway point, and Scale is blended linearly
// Leaf: InterpolateTo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF4100, 312 bytes, `ret 8`. PDB: ?InterpolateTo@CParticleSystem@NParticleEngine@@QAEXABV12@M@Z.
// Callees: CPSCBase virtual slot 5 (+0x14, the component type; descriptive name) and slot 14 (+0x38)
//   InterpolateTo(const CPSCBase&, float) (PDB, the CPSC* overrides).
// Layout: Ego_r.pdb CParticleSystem with the retail container sizes (CVectorMap 0x10, STLport lists 4 bytes,
//   CParticleUpdateParameters 0x24): Components +0x0C, Particles +0x1C, Decals +0x20, UpdateParameters +0x24, the flag byte
//   +0x50 (Initialised, Enabled, Dead, NormalisedTimeEnabled, UseAllAttractors, Visible, StayWithEmitter, ScaleParticles),
//   GlobalAlpha +0x54, Scale +0x60. 0x0122F59C is 0.5f, 0x0122DED8 1.0f.
// Shape: the two component vectors are walked in step (the other system's cursor is taken first); per-iteration
//   references to both instances give the extra induction pointers. `t > 0.5f ? other.X : X` selects the byte, then the
//   bit-field insert. Scale is `Scale * (1.0f - t) + other.Scale * t` through inline C3DVector operators (the right operand
//   of `+` is evaluated first).
namespace NParticleEngine {

class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetType() const;                                        // slot 5
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void InterpolateTo(const CPSCBase& other, float t);          // slot 14
};

template <class T> struct CCountedPointer {
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
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
    virtual ~CParticleSystem();
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