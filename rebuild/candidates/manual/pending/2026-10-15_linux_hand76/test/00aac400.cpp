#include <cstdio>
struct CSoundPlayer { long m_stops; long m_last; virtual void Stop(long handle) { ++m_stops; m_last = handle; } };
struct CSoundSystem { CSoundPlayer* m_player; virtual CSoundPlayer* GetPlayer() { return m_player; } };
struct CSoundManagerLH { CSoundSystem* m_pSystem; };
struct CActiveSound { long m_handle; };
struct SNode { SNode* next; SNode* prev; CActiveSound* sound; long extra; };
static int g_prunes = 0;
struct CSoundEmitterLH {
    CSoundManagerLH* m_pManager; SNode* m_pSounds;
    void RemoveInactiveSounds() { ++g_prunes; }
    void vfunc_9(long handle)
    {
        if (m_pManager->m_pSystem) {
            for (SNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
                CActiveSound* active = node->sound;
                if (active->m_handle == handle) {
                    m_pManager->m_pSystem->GetPlayer()->Stop(handle);
                    active->m_handle = -1;
                }
            }
            RemoveInactiveSounds();
        }
    }
};
int main()
{
    CActiveSound a, b, c; a.m_handle = 5; b.m_handle = 6; c.m_handle = 5;
    SNode head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head; n1.sound = &a; n2.sound = &b; n3.sound = &c;
    CSoundPlayer p; p.m_stops = 0; p.m_last = 0;
    CSoundSystem s; s.m_player = &p;
    CSoundManagerLH mgr; mgr.m_pSystem = 0;
    CSoundEmitterLH e; e.m_pManager = &mgr; e.m_pSounds = &head;
    e.vfunc_9(5);
    bool ok = g_prunes == 0 && p.m_stops == 0 && a.m_handle == 5;
    mgr.m_pSystem = &s;
    e.vfunc_9(5);
    ok = ok && g_prunes == 1 && p.m_stops == 2 && p.m_last == 5 && a.m_handle == -1 && b.m_handle == 6 && c.m_handle == -1;
    if (!ok) { std::printf("00aac400_TEST FAIL\n"); return 1; }
    std::printf("00aac400_TEST PASS\n");
    return 0;
}
