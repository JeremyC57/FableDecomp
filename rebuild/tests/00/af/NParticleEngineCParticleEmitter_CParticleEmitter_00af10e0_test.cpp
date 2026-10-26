// Behaviour test for 0x00AF10E0 NParticleEngine::CParticleEmitter::~CParticleEmitter: reverse member destruction, handle notify + release.
#include <cstdio>
#include <cstring>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
    long m_refCount; int notified, destroyed;
    void Destroy() { ++destroyed; }
    void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++notified; }
};
struct CIVCountedPointer {
    CEnginePersistentPrimitiveBase* m_p;
    CIVCountedPointer() : m_p(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release() { CEnginePersistentPrimitiveBase* p = m_p; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_p = 0; } }
    CIVCountedPointer& operator=(CEnginePersistentPrimitiveBase* p) { Release(); m_p = p; return *this; }
    CEnginePersistentPrimitiveBase* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};
class CEnginePrimitiveHandle { public:
    CIVCountedPointer Data;
    ~CEnginePrimitiveHandle() { if (Data.GetRefCount() == 1) Data->OnLastHandleReleased(this); Data = 0; Log('H'); }
};
class CCharString { public: ~CCharString() { Log('N'); } };
class CBaseObject { public: ~CBaseObject() { Log('B'); } };
struct SParticleSystemVector { ~SParticleSystemVector() { Log('V'); } };
struct SUpdateParameters { ~SUpdateParameters() { Log('U'); } };
class C3DOrientationInterpolator { public: ~C3DOrientationInterpolator() { Log('O'); } };
class C3DPositionInterpolator { public: ~C3DPositionInterpolator() { Log('P'); } };
struct CParticleEmitter : CBaseObject {
    SParticleSystemVector ParticleSystems; SUpdateParameters UpdateParameters; CEnginePrimitiveHandle PrimitiveGroupHandle;
    C3DOrientationInterpolator OrientationInterpolator; C3DPositionInterpolator PositionInterpolator; CCharString Name;
    ~CParticleEmitter() {}
};
int main()
{
    CEnginePersistentPrimitiveBase prim = {1, 0, 0};
    { CParticleEmitter e; e.PrimitiveGroupHandle.Data.m_p = &prim; }
    if (std::strcmp(g_log, "NPOHUVB") || prim.notified != 1 || prim.destroyed != 1 || prim.m_refCount != 0) {
        std::printf("00af10e0_TEST FAIL\n"); return 1;
    }
    std::printf("00af10e0_TEST PASS\n");
    return 0;
}