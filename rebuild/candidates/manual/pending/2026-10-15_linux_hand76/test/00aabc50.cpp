#include <cstdio>
struct CSoundSystem;
struct CSoundManagerLH { CSoundSystem* m_pSystem; };
struct CActiveSound { long m_handle; long m_sound; long m_instance; };
struct SNode { SNode* next; SNode* prev; CActiveSound* sound; long extra; };
struct CSoundEmitterLH {
    CSoundManagerLH* m_pManager; SNode* m_pSounds;
    bool vfunc_23(long sound, long instance)
    {
        if (m_pManager->m_pSystem) {
            for (SNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
                CActiveSound* active = node->sound;
                if (active->m_sound == sound && active->m_instance == instance)
                    return true;
            }
        }
        return false;
    }
};
int main()
{
    CActiveSound s1, s2; s1.m_sound = 1; s1.m_instance = 10; s2.m_sound = 2; s2.m_instance = 20;
    SNode head, n1, n2;
    head.next = &n1; n1.next = &n2; n2.next = &head; n1.sound = &s1; n2.sound = &s2;
    CSoundManagerLH mgr; mgr.m_pSystem = 0;
    CSoundEmitterLH e; e.m_pManager = &mgr; e.m_pSounds = &head;
    bool ok = !e.vfunc_23(2, 20);
    mgr.m_pSystem = (CSoundSystem*)&mgr;
    ok = ok && e.vfunc_23(2, 20) && e.vfunc_23(1, 10) && !e.vfunc_23(1, 20) && !e.vfunc_23(3, 10);
    if (!ok) { std::printf("00aabc50_TEST FAIL\n"); return 1; }
    std::printf("00aabc50_TEST PASS\n");
    return 0;
}
