// Behaviour test for CSoundEmitterLH::vfunc_8 (0x00AAC340): matching idle sounds are stopped and marked -1, the list is pruned, the remaining matches are queried; nothing happens without a sound system.
#include <cstdio>
struct CSoundPlayer {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {}
    virtual void Stop(long handle);                    // slot 22 (+0x58)
    virtual void v23() {} virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {}
    virtual float Query(long handle, long* out);       // slot 28 (+0x70)
};

struct CSoundSystem {
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual CSoundPlayer* GetPlayer();                 // slot 6 (+0x18)
};

struct CSoundManagerLH {
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                           // +0x40
};

struct CActiveSound {                                  // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00() {}
    long          m_references;                        // +0x04
    long          m_handle;                            // +0x08
    void*         m_pSource;                           // +0x0c
    void*         m_pD;                                // +0x10
    bool          m_e;                                 // +0x14
    bool          m_f;                                 // +0x15
    unsigned char _pad16[2];
    long          m_id;                                // +0x18
};

struct SSoundNode {                                    // VC6 list node, 16 bytes
    SSoundNode*   next;
    SSoundNode*   prev;
    CActiveSound* sound;                               // +0x08
    long          extra;                               // +0x0c
};

struct CSoundEmitterLH {                               // vtable 0x0129E6EC
    virtual void v00() {}
    CSoundManagerLH* m_pManager;                       // +0x04
    float            m_position[3];                    // +0x08
    SSoundNode*      m_pSounds;                        // +0x14: list sentinel
    void RemoveInactiveSounds();                       // 0x00AABCA0
    virtual void vfunc_8(void* source, long id);
};

void CSoundEmitterLH::vfunc_8(void* source, long id)
{
    if (m_pManager->m_pSystem) {
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_references == 0 && sound->m_pSource == source && sound->m_id == id) {
                m_pManager->m_pSystem->GetPlayer()->Stop(sound->m_handle);
                sound->m_handle = -1;
            }
        }
        RemoveInactiveSounds();
        for (SSoundNode* node = m_pSounds->next; node != m_pSounds; node = node->next) {
            CActiveSound* sound = node->sound;
            if (sound->m_references == 0 && sound->m_pSource == source && sound->m_id == id) {
                long out;
                m_pManager->m_pSystem->GetPlayer()->Query(sound->m_handle, &out);
            }
        }
    }
}
static long g_stopped[8]; static int g_stops = 0; static long g_queried[8]; static int g_queries = 0; static int g_prunes = 0; static bool g_pruneRemoves = true;
void CSoundPlayer::Stop(long handle) { g_stopped[g_stops++] = handle; }
float CSoundPlayer::Query(long handle, long* out) { *out = 0; g_queried[g_queries++] = handle; return 1.0f; }
static CSoundPlayer g_player;
CSoundPlayer* CSoundSystem::GetPlayer() { return &g_player; }
void CSoundEmitterLH::RemoveInactiveSounds()
{
    ++g_prunes;
    for (SSoundNode* n = m_pSounds->next; n != m_pSounds; n = n->next)
        if (g_pruneRemoves && n->sound->m_handle == -1) { n->prev->next = n->next; n->next->prev = n->prev; }
}
int main()
{
    CSoundSystem system; CSoundManagerLH manager; manager.m_pSystem = &system;
    static unsigned char emem[sizeof(CSoundEmitterLH)]; CSoundEmitterLH* e = (CSoundEmitterLH*)emem;
    static unsigned char smem[5][sizeof(CActiveSound)]; CActiveSound* s[5];
    int src = 0, other = 0;
    SSoundNode head, nodes[5];
    head.next = head.prev = &head;
    for (int i = 0; i < 5; ++i) {
        s[i] = (CActiveSound*)smem[i]; s[i]->m_references = 0; s[i]->m_handle = 100 + i; s[i]->m_pSource = &src; s[i]->m_id = 7;
        nodes[i].sound = s[i]; nodes[i].prev = head.prev; nodes[i].next = &head; head.prev->next = &nodes[i]; head.prev = &nodes[i];
    }
    s[1]->m_references = 1;     /* not idle */
    s[2]->m_pSource = &other;   /* other source */
    s[3]->m_id = 8;             /* other id */
    e->m_pManager = &manager; e->m_pSounds = &head;
    e->CSoundEmitterLH::vfunc_8(&src, 7);
    bool ok = g_stops == 2 && g_stopped[0] == 100 && g_stopped[1] == 104 && s[0]->m_handle == -1 && s[4]->m_handle == -1
        && s[1]->m_handle == 101 && g_prunes == 1 && g_queries == 0;   /* both matches were pruned */
    /* matches that survive the prune are queried with their (now -1) handle */
    s[1]->m_references = 0; s[1]->m_handle = 201; g_pruneRemoves = false;
    e->CSoundEmitterLH::vfunc_8(&src, 7);
    ok = ok && g_stops == 3 && g_stopped[2] == 201 && g_prunes == 2 && g_queries == 1 && g_queried[0] == -1;
    manager.m_pSystem = 0;
    e->CSoundEmitterLH::vfunc_8(&src, 7);
    ok = ok && g_stops == 3 && g_prunes == 2 && g_queries == 1;   /* no sound system: nothing */
    if (!ok) { std::printf("00aac340_TEST FAIL\n"); return 1; }
    std::printf("00aac340_TEST PASS\n");
    return 0;
}
