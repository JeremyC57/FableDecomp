// Behaviour test for 0x00770350 std::__destroy_aux over std::list<CWeaponTrailEntry>
#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static void CountedFree(void* p) { ++g_frees; std::free(p); }

struct __false_type { __false_type() { } };
struct SNodeBase { SNodeBase* m_next; SNodeBase* m_prev; };
struct SNode : SNodeBase { float Data[7]; };
struct CTrailList {
    SNodeBase* m_node;
    void Init() { m_node = (SNodeBase*)std::malloc(sizeof(SNode)); m_node->m_next = m_node; m_node->m_prev = m_node; }
    void PushBack()
    {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        tmp->m_next = m_node; tmp->m_prev = m_node->m_prev; m_node->m_prev->m_next = tmp; m_node->m_prev = tmp;
    }
    void clear()
    {
        SNodeBase* cur = m_node->m_next;
        while (cur != m_node) {
            SNodeBase* tmp = cur;
            cur = cur->m_next;
            CountedFree(tmp);
        }
        m_node->m_next = m_node;
        m_node->m_prev = m_node;
    }
    void Destroy()
    {
        clear();
        if (m_node != 0)
            CountedFree(m_node);
    }
};

static void DestroyAux(CTrailList* first, CTrailList* last, const __false_type&)
{
    for (; first != last; ++first)
        first->Destroy();
}

int main()
{
    CTrailList lists[3];
    for (int i = 0; i < 3; ++i) {
        lists[i].Init();
        for (int k = 0; k < i * 2; ++k)
            lists[i].PushBack();       // 0, 2 and 4 nodes
    }
    DestroyAux(lists, lists + 3, __false_type());
    if (g_frees != 3 + 6) { std::printf("00770350_TEST FAIL\n"); return 1; }
    DestroyAux(lists, lists, __false_type());
    if (g_frees != 9) { std::printf("00770350_TEST FAIL\n"); return 1; }
    std::printf("00770350_TEST PASS\n");
    return 0;
}