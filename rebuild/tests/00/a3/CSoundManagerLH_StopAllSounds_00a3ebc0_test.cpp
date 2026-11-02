// Test: CSoundManagerLH::StopAllSounds() stops every emitter on the STLport emitter list (sentinel node), then the manager's own emitter.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CSoundEmitterLH { public:
    int Id;
    virtual ~CSoundEmitterLH() { }
    virtual void Shutdown() { g_order[g_count++] = Id; }
};
struct SListNode {
    SListNode*       _M_next;
    SListNode*       _M_prev;
    CSoundEmitterLH* _M_data;
};
class CEmitterList { public:                     // STLport list<CSoundEmitterLH*>: the sentinel node pointer only
    SListNode* _M_node;
};
class CSoundManagerLH { public:
    CSoundEmitterLH* m_pMain;
    CEmitterList     Sounds;
    void StopAllSounds();
};
void CSoundManagerLH::StopAllSounds()
{
    for (SListNode* it = Sounds._M_node->_M_next; it != Sounds._M_node; it = it->_M_next)
        it->_M_data->Shutdown();
    m_pMain->Shutdown();
}
int main()
{
    CSoundEmitterLH main, a, b;
    main.Id = 9; a.Id = 1; b.Id = 2;
    SListNode sentinel, na, nb;
    sentinel._M_next = &na; na._M_next = &nb; nb._M_next = &sentinel;
    na._M_data = &a; nb._M_data = &b;
    CSoundManagerLH manager;
    manager.m_pMain = &main;
    manager.Sounds._M_node = &sentinel;
    manager.StopAllSounds();
    if (g_count != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 9) { std::printf("00a3ebc0_TEST FAIL\n"); return 1; }
    sentinel._M_next = &sentinel; g_count = 0;   // empty list: only the main emitter
    manager.StopAllSounds();
    if (g_count != 1 || g_order[0] != 9) { std::printf("00a3ebc0_TEST FAIL\n"); return 1; }
    std::printf("00a3ebc0_TEST PASS\n");
    return 0;
}