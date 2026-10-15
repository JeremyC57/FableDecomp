// CParticleEmitter::ClearAllPrimitives(CIEngine*) -- when the primitive group exists: every particle system clears its primitives from the group, the engine (if given) removes the group, then the group handle is cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEE940, length 128 bytes.
// Leaf: ClearAllPrimitives
// Callees: CParticleSystem::ClearAllPrimitives(CEnginePrimitiveHandle*) @ 0x00AF4320, the engine's virtual slot 31 (+0x7c,
// remove primitive), the primitive's slot 3 (last-handle notification) and slot 1 (Destroy).
// Layout (retail, as the landed CParticleEmitter rows): ParticleSystems +0x08, PrimitiveGroupHandle +0x38 (vptr + counted Data).
// Shape: the guard reads the handle's Data pointer; the handle's address is formed in ebx for the loop, again in edx for the
// engine call and in esi for the inlined CEnginePrimitiveHandle::Clear (null-safe GetRefCount() == 1 -> slot 3, release, then the
// second unconditional zero store of the default-pointer assignment).
struct CEnginePrimitiveHandle;
struct CEnginePrimitiveBase {               // counted primitive: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    virtual void v02();
    virtual void OnLastHandleCleared(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                        // +0x04
};
struct CIVCountedPointer_Primitive {
    CEnginePrimitiveBase* m_p;
    CIVCountedPointer_Primitive() : m_p(0) { }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
    void Release()
    {
        CEnginePrimitiveBase* object = m_p;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer_Primitive& operator=(const CIVCountedPointer_Primitive& other)
    {
        Release();
        m_p = other.m_p;
        if (m_p)
            ++m_p->m_refCount;
        return *this;
    }
};
struct CBase {
    virtual void v00();
};
struct CEnginePrimitiveHandle : CBase {     // vtable 0x0122F598
    CIVCountedPointer_Primitive Data;       // +4
    void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.m_p->OnLastHandleCleared(this);
        Data = CIVCountedPointer_Primitive();
    }
};
struct C3DVector {
    float X, Y, Z;
};
struct CParticleSystem;
struct CParticleSystemPtr {                 // CCountedPointer<CParticleSystem> (8 bytes)
    CParticleSystem* m_pObject;
    void*            m_pCount;
};
struct CParticleSystemPtrVector {           // retail vector<CCountedPointer<CParticleSystem> > (12 bytes)
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
};
#pragma pack(push, 1)
struct CParticleEmitter {
    virtual void v00();
    unsigned char            _base4[4];                     // +0x04
    CParticleSystemPtrVector ParticleSystems;               // +0x08
    unsigned char            UpdateParameters[0x24];        // +0x14 CParticleUpdateParameters<unsigned long,unsigned long>
    CEnginePrimitiveHandle   PrimitiveGroupHandle;          // +0x38
    unsigned short           Priority                   : 3;   // +0x40 bits 0..2
    unsigned short           IsScreenDisplacement       : 1;   //       bit 3
    unsigned short           ReadZBuffer                : 1;   //       bit 4
    unsigned short           Dead                       : 1;   //       bit 5
    unsigned short           ParticleCreationDisabled   : 1;   //       bit 6
    unsigned short           ContinuousEmitter          : 1;   //       bit 7
    unsigned short           Initialised                : 1;   //       bit 8
    unsigned short           WaitingToSleep             : 1;   //       bit 9
    unsigned short           Sleeping                   : 1;   //       bit 10
    unsigned short           Emitter2D                  : 1;   //       bit 11
    unsigned short           Emitter2DLayer             : 7;   // +0x42 bits 0..6
    unsigned char            PreWaterEffect             : 1;   // +0x44 bit 0
    unsigned char            WaterEffect                : 1;
    unsigned char            ZBufferWriteable           : 1;
    unsigned char            Visible                    : 1;
    unsigned char            AlwaysUpdate               : 1;
    unsigned char            DieIfOffsceen              : 1;
    unsigned char            OffscreenUpdate            : 1;
    unsigned char            ClipEffectToWeatherMask    : 1;
    unsigned char            EnableDithering            : 1;   // +0x45 bit 0
    unsigned char            CalcBoundingSphereOnceOnly : 1;
    unsigned char            BoundingSphereValid        : 1;
    unsigned char            ClearedParticles           : 1;
    unsigned char            ClearedDecals              : 1;
    C3DVector                OldPos;                        // +0x46
    C3DVector                Velocity;                      // +0x52
    void ClearAllPrimitives(struct CIEngine* engine);
};
#pragma pack(pop)
struct CParticleSystem {
    void ClearAllPrimitives(CEnginePrimitiveHandle* group);    // 0x00AF4320
};
struct CIEngine {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15)
    V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30)
#undef V
    virtual void RemovePrimitive(CEnginePrimitiveHandle* handle);   // slot 31 (+0x7c)
};
void CParticleEmitter::ClearAllPrimitives(CIEngine* engine)
{
    if (PrimitiveGroupHandle.Data.m_p) {
        for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it)
            it->m_pObject->ClearAllPrimitives(&PrimitiveGroupHandle);
        if (engine)
            engine->RemovePrimitive(&PrimitiveGroupHandle);
        PrimitiveGroupHandle.Clear();
    }
}