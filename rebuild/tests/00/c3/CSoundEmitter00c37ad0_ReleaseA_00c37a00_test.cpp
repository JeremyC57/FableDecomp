// Behaviour test for CSoundEmitter_00c37ad0::ReleaseA @ 0x00C37A00: the emitter is handed to its owning registry (whatever the handle).
#include <cstdio>
struct CInstance;
static CInstance* g_destroyed = 0; static int g_calls = 0;
class CInstanceRegistry {
public:
    void DestroyInstance(CInstance* instance) { g_destroyed = instance; ++g_calls; }
};
class CSoundEmitter_00c37ad0 {
public:
    virtual void* v00() { return 0; }
    virtual void ReleaseA(long handle);
    virtual void SetScalar(long, float) { }
    virtual void v03(long, long) { }
    virtual void ReleaseB(long handle);
    CInstanceRegistry* m_pOwner;
};
void CSoundEmitter_00c37ad0::ReleaseA(long handle)
{
    m_pOwner->DestroyInstance((CInstance*)this);
}
void CSoundEmitter_00c37ad0::ReleaseB(long) { }
int main()
{
    CInstanceRegistry registry;
    CSoundEmitter_00c37ad0 emitter;
    emitter.m_pOwner = &registry;
    emitter.ReleaseA(1234);
    if (g_calls != 1 || g_destroyed != (CInstance*)&emitter) { std::printf("00c37a00_TEST FAIL\n"); return 1; }
    std::printf("00c37a00_TEST PASS\n");
    return 0;
}