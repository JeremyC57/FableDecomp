// Behaviour test for 00aee8c0: body copied from src/00aee8c0.cpp (CParticleEmitter::Kill) with the inline handle Clear and counted
// pointer; CParticleSystem::Kill and the primitive virtuals stubbed to record calls.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CEnginePrimitiveHandle;
static int g_destroy = 0, g_lastCleared = 0; static CEnginePrimitiveHandle* g_lastHandle = 0;
struct CEnginePrimitiveBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroy; }
    virtual void v02() {}
    virtual void OnLastHandleCleared(CEnginePrimitiveHandle* handle) { ++g_lastCleared; g_lastHandle = handle; }
    long m_refCount;
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
    virtual void v00() {}
};
class CEnginePrimitiveHandle : public CBase { public:
    CIVCountedPointer_Primitive Data;
    void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.m_p->OnLastHandleCleared(this);
        Data = CIVCountedPointer_Primitive();
    }
};
class C3DVector { public:
    float X, Y, Z;
    void Set(float x, float y, float z) { Z = z; Y = y; X = x; }
};
static int g_sysKill = 0;
struct CParticleSystem {
    void Kill() { ++g_sysKill; }
};
struct CParticleSystemPtr {
    CParticleSystem* m_pObject;
    void*            m_pCount;
};
struct CParticleSystemPtrVector {
    CParticleSystemPtr* m_first;
    CParticleSystemPtr* m_last;
    CParticleSystemPtr* m_end;
};
#pragma pack(push, 1)
struct CParticleEmitter {
    virtual void v00() {}
    unsigned char            _base4[4];
    CParticleSystemPtrVector ParticleSystems;
    unsigned char            UpdateParameters[0x24];
    CEnginePrimitiveHandle   PrimitiveGroupHandle;
    unsigned short           Priority                   : 3;
    unsigned short           IsScreenDisplacement       : 1;
    unsigned short           ReadZBuffer                : 1;
    unsigned short           Dead                       : 1;
    unsigned short           ParticleCreationDisabled   : 1;
    unsigned short           ContinuousEmitter          : 1;
    unsigned short           Initialised                : 1;
    unsigned short           WaitingToSleep             : 1;
    unsigned short           Sleeping                   : 1;
    unsigned short           Emitter2D                  : 1;
    unsigned short           Emitter2DLayer             : 7;
    unsigned char            PreWaterEffect             : 1;
    unsigned char            WaterEffect                : 1;
    unsigned char            ZBufferWriteable           : 1;
    unsigned char            Visible                    : 1;
    unsigned char            AlwaysUpdate               : 1;
    unsigned char            DieIfOffsceen              : 1;
    unsigned char            OffscreenUpdate            : 1;
    unsigned char            ClipEffectToWeatherMask    : 1;
    unsigned char            EnableDithering            : 1;
    unsigned char            CalcBoundingSphereOnceOnly : 1;
    unsigned char            BoundingSphereValid        : 1;
    unsigned char            ClearedParticles           : 1;
    unsigned char            ClearedDecals              : 1;
    C3DVector                OldPos;
    C3DVector                Velocity;
    void Kill();
};
#pragma pack(pop)
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
int main()
{
    CHECK((char*)&((CParticleEmitter*)0)->OldPos == (char*)0 + 0x46);
    CHECK((char*)&((CParticleEmitter*)0)->Velocity == (char*)0 + 0x52);
    CParticleEmitter e; memset(e._base4, 0, sizeof(e) - 4);
    CParticleSystem systems[3]; CParticleSystemPtr ptrs[3];
    for (int i = 0; i < 3; ++i) { ptrs[i].m_pObject = &systems[i]; ptrs[i].m_pCount = 0; }
    e.ParticleSystems.m_first = ptrs; e.ParticleSystems.m_last = ptrs + 3; e.ParticleSystems.m_end = ptrs + 3;
    CEnginePrimitiveBase prim; prim.m_refCount = 1;
    e.PrimitiveGroupHandle.Data.m_p = &prim;
    e.Initialised = 1; e.Priority = 5; e.Emitter2D = 1; e.Sleeping = 1;
    e.OldPos.X = 1; e.OldPos.Y = 2; e.OldPos.Z = 3; e.Velocity.X = 4; e.Velocity.Y = 5; e.Velocity.Z = 6;
    e.Kill();
    CHECK(g_sysKill == 3);
    CHECK(g_lastCleared == 1 && g_lastHandle == &e.PrimitiveGroupHandle);    // sole handle: last-handle notification
    CHECK(g_destroy == 1); CHECK(e.PrimitiveGroupHandle.Data.m_p == 0);     // then released to zero -> Destroy
    CHECK(e.Dead == 1); CHECK(e.Initialised == 0);
    CHECK(e.Priority == 5 && e.Emitter2D == 1 && e.Sleeping == 1);          // neighbouring bits untouched
    CHECK(e.OldPos.X == 0 && e.OldPos.Y == 0 && e.OldPos.Z == 0 && e.Velocity.X == 0 && e.Velocity.Y == 0 && e.Velocity.Z == 0);
    e.Kill();                                                               // already dead: nothing happens
    CHECK(g_sysKill == 3 && g_destroy == 1);
    // a second emitter whose handle shares the primitive with another holder: no notification, no destroy
    CParticleEmitter f; memset(f._base4, 0, sizeof(f) - 4);
    f.ParticleSystems.m_first = ptrs; f.ParticleSystems.m_last = ptrs;
    CEnginePrimitiveBase prim2; prim2.m_refCount = 2; f.PrimitiveGroupHandle.Data.m_p = &prim2;
    f.Kill();
    CHECK(g_sysKill == 3); CHECK(g_lastCleared == 1); CHECK(g_destroy == 1); CHECK(prim2.m_refCount == 1); CHECK(f.PrimitiveGroupHandle.Data.m_p == 0); CHECK(f.Dead == 1);
    if (fails == 0) printf("00aee8c0_TEST PASS\n"); else printf("00aee8c0_TEST FAIL\n");
    return fails;
}