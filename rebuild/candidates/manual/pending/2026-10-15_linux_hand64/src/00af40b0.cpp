// CParticleSystem::PreparePrimitives(float, CEnginePrimitivePrepareInfo*, CEnginePrimitiveHandle*) -- forwards PreparePrimitives to every enabled component of the component map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF40B0, 71 bytes (`ret 0Ch`).
// Callees: the component's virtual slot 13 (`call [eax+34h]`): (time, info, this, &this->UpdateParameters, handle).
// Shape: the handle is cached in ebp before the loop, time and info are re-read from their argument slots for
// every call. Loop: the component-map walk of EnterTimelineEnd (0x00AF4070) with the end compared from memory at
// entry (`cmp ebx,[esi+10h]`: no end local before the test).

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
    void PreparePrimitives(float time, CEnginePrimitivePrepareInfo* info, CEnginePrimitiveHandle* handle);
};

void CParticleSystem::PreparePrimitives(float time, CEnginePrimitivePrepareInfo* info, CEnginePrimitiveHandle* handle)
{
    SComponentEntry* it = m_componentsFirst;
    if (it != m_componentsLast) {
        SComponentState* state = &it->second;
        SComponentEntry* last;
        do {
            if (state->Enabled)
                state->Component->PreparePrimitives(time, info, this, &UpdateParameters, handle);
            last = m_componentsLast;
            ++it;
            state = (SComponentState*)((char*)state + sizeof(SComponentEntry));
        } while (it != last);
    }
}
