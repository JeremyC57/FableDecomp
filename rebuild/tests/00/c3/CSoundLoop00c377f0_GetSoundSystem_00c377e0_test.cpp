// Test for 0x00C377E0 CSoundLoop_00c377f0::GetSoundSystem: owner -> host -> core -> sound system.
#include <cstdio>
struct CSys { int id; };
struct CCore { char pad[0x38]; CSys* m_pSoundSystem; };
struct CHost { int pad; CCore* m_pCore; };
struct COwner { CHost* m_pHost; };
struct CLoop { COwner* m_pOwner; CSys* GetSoundSystem() { return m_pOwner->m_pHost->m_pCore->m_pSoundSystem; } };
int main()
{
    CSys s = {7}; CCore c; c.m_pSoundSystem = &s; CHost h; h.m_pCore = &c; COwner o; o.m_pHost = &h; CLoop l; l.m_pOwner = &o;
    if (l.GetSoundSystem() != &s) { std::printf("00c377e0_TEST FAIL\n"); return 1; }
    std::printf("00c377e0_TEST PASS\n");
    return 0;
}