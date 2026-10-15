// CParticleSystem::GetBoundingSphereWorldSpace(C3DBoundingSphere*, float, CParticleUpdateParameters*) -- zeroes the sphere radius, then ORs together GetBoundingSphereWorldSpace of every enabled component
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3FB0, 85 bytes (`ret 0Ch`).
// Callees: the component's virtual slot 19 (`call [edx+4Ch]`): (sphere, time, this, &this->UpdateParameters).
// Shape: the PDB signature's CParticleUpdateParameters* argument is not used: the components get the system's own
// UpdateParameters member (+0x24). C3DBoundingSphere = centre (12 bytes) + Radius (+0x0c), zeroed with an
// immediate store (0.0f). The bool result accumulates in bl (`or bl,al`). Loop: the component-map walk of
// CParticleSystem::EnterTimelineEnd (0x00AF4070), entry pointer in ebp, state pointer in edi.

struct CIEngine;
struct C3DBoundingSphere;
struct CEnginePrimitivePrepareInfo;
struct CEnginePrimitiveHandle;
struct CParticleSystem;
struct CParticleUpdateParameters { unsigned char m_data[0x24]; };   // retail 0x24 bytes (PDB 0x2c)

namespace NParticleEngine {
struct CPSCBase {                                    // particle-system component; slots by retail call offsets
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void PreparePrimitives(float time, CEnginePrimitivePrepareInfo* info, CParticleSystem* system,
                                   CParticleUpdateParameters* params, CEnginePrimitiveHandle* handle);   // slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float time, CParticleSystem* system,
                                             CParticleUpdateParameters* params);                        // slot 19 (+0x4c)
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void EnterTimelineEnd();                                                                     // slot 25 (+0x64)
    virtual long GetNoActiveDecals();                                                                    // slot 26 (+0x68)
    virtual void v27();
    virtual bool Initialise(CIEngine* engine, CParticleSystem* system, CParticleUpdateParameters* params,
                            CEnginePrimitiveHandle* handle);                                             // slot 28 (+0x70)
    virtual void ClearAllPrimitives(CIEngine* engine, CEnginePrimitiveHandle* handle);                  // slot 29 (+0x74)
    virtual void v30();
    virtual float GetApproximateSplineLength();                                                          // slot 31 (+0x7c)
};

struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    T* operator->() const { return Object; }
};
}

// Tail of NParticleEngine::CComponentInstance after its vptr (PDB: vptr +0, Component +4, Enabled +0xc,
// InstanceName +0x10). The retail loops walk a pointer to this part in step with the map iterator.
struct SComponentState {
    NParticleEngine::CCountedPointer<NParticleEngine::CPSCBase> Component;   // entry +0x08
    bool          Enabled;                                                  // entry +0x10
    unsigned long InstanceName;                                             // entry +0x14
};

struct SComponentEntry {                             // pair<EPSCType, CComponentInstance>, 24 bytes
    long            first;                           // +0x00 EPSCType
    void*           m_instanceVtable;                // +0x04 CComponentInstance vptr
    SComponentState second;                          // +0x08
};

// CParticleSystem, retail layout (PDB names; the retail CVectorMap is 16 bytes and the STLport lists 4 bytes).
struct CParticleSystem {
    void*                     m_vtable;              // +0x00
    void*                     PParentEmitter;        // +0x04
    CIEngine*                 PEngine;               // +0x08
    SComponentEntry*          m_componentsFirst;     // +0x0c  Components (CVectorMap: first/last/end, compare, dirty)
    SComponentEntry*          m_componentsLast;      // +0x10
    SComponentEntry*          m_componentsEnd;       // +0x14
    unsigned char             m_componentsCompare;   // +0x18
    bool                      m_componentsDirty;     // +0x19
    void*                     Particles;             // +0x1c  (STLport list head node)
    void*                     Decals;                // +0x20
    CParticleUpdateParameters UpdateParameters;      // +0x24
    long                      NoActiveParticles;     // +0x48
    long                      NoActiveDecals;        // +0x4c
    bool                      Initialised : 1;       // +0x50 bit 0
    bool                      Enabled : 1;
    bool                      Dead : 1;
    bool                      NormalisedTimeEnabled : 1;
    bool                      UseAllAttractors : 1;
    bool                      Visible : 1;
    bool                      StayWithEmitter : 1;
    bool                      ScaleParticles : 1;    // bit 7
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float time, CParticleUpdateParameters* params);
};

struct C3DBoundingSphere {
    float Centre[3];
    float Radius;                                    // +0x0c
};

bool CParticleSystem::GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float time, CParticleUpdateParameters* params)
{
    sphere->Radius = 0.0f;
    bool found = false;
    SComponentEntry* it = m_componentsFirst;
    SComponentEntry* last = m_componentsLast;
    if (it != last) {
        SComponentState* state = &it->second;
        do {
            if (state->Enabled)
                found |= state->Component->GetBoundingSphereWorldSpace(sphere, time, this, &UpdateParameters);
            last = m_componentsLast;
            ++it;
            state = (SComponentState*)((char*)state + sizeof(SComponentEntry));
        } while (it != last);
    }
    return found;
}