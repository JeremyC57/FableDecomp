// CParticleEmitter::Kill() -- once (Dead bit clear): kill every particle system, clear the primitive group handle, set Dead, zero OldPos and Velocity, clear Initialised
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEE8C0, length 117 bytes (padded to 128).
// Leaf: Kill
// Callees: CParticleSystem::Kill() @ 0x00AF5870, the primitive's virtual slot 3 (last-handle notification) and slot 1 (Destroy).
// Layout (retail, as the landed CParticleEmitter rows): ParticleSystems +0x08, PrimitiveGroupHandle +0x38, bit-field group
// +0x40 (Dead bit 5, Initialised bit 8 = byte +0x41 bit 0), OldPos +0x46 and Velocity +0x52 (unaligned: pack(1)).
// Shape: `last` re-read each pass; CEnginePrimitiveHandle::Clear inlined (shared zero register ebp); the vectors are zeroed
// through an inline `Set(x, y, z)` member storing Z, Y, X (stores run from the highest address down) -- stores through the
// member's `this` keep the two bit writes in place (`or byte ptr [+40h],20h` before them, `and byte ptr [+41h],0FEh` after);
// chained `X = Y = Z = 0.0f` member stores let the lane hoist both bit loads above the zeros and sink the stores. The
// bit-field group is the PDB `unsigned short` run (bits 0..11 at +0x40): with `unsigned char` units the final clear is a
// load/and/store split around `pop edi`.
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
    void Set(float x, float y, float z) { Z = z; Y = y; X = x; }
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
    void Kill();
};
#pragma pack(pop)
struct CParticleSystem {
    void Kill();                            // 0x00AF5870
};
void CParticleEmitter::Kill()
{
    if (!Dead) {
        for (CParticleSystemPtr* it = ParticleSystems.m_first; it != ParticleSystems.m_last; ++it)
            it->m_pObject->Kill();
        PrimitiveGroupHandle.Clear();
        Dead = 1;
        OldPos.Set(0.0f, 0.0f, 0.0f);
        Velocity.Set(0.0f, 0.0f, 0.0f);
        Initialised = 0;
    }
}