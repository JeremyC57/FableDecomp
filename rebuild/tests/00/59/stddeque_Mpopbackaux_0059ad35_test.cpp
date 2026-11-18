// Behaviour test for 0059ad35 (CRetailDeque_0059ad35::PopBackAux): the STLport deque back aux step frees the emptied buffer and moves to the neighbouring map node.
#include <cstdio>

static void* g_freed = 0;

struct SElem { long m_words[1]; };

struct CDeque {
    struct iterator {
        SElem*  m_pCur;
        SElem*  m_pFirst;
        SElem*  m_pLast;
        SElem** m_pNode;
        void SetNode(SElem** node)
        {
            m_pNode = node;
            m_pFirst = *node;
            m_pLast = m_pFirst + 0x80 / sizeof(SElem);
        }
    };
    iterator m_start;
    iterator m_finish;
    SElem**  m_pMap;
    unsigned long m_mapSize;
    void DeallocateNode(SElem* p) { if (p != 0) g_freed = p; }
    void PopFrontAux()
    {
        DeallocateNode(m_start.m_pFirst);
        m_start.SetNode(m_start.m_pNode + 1);
        m_start.m_pCur = m_start.m_pFirst;
    }
    void PopBackAux()
    {
        DeallocateNode(m_finish.m_pFirst);
        m_finish.SetNode(m_finish.m_pNode - 1);
        m_finish.m_pCur = m_finish.m_pLast - 1;
    }
};

int main()
{
    static SElem buf0[0x80 / sizeof(SElem)], buf1[0x80 / sizeof(SElem)];
    SElem* map[2] = { buf0, buf1 };
    CDeque d;
    d.m_pMap = map; d.m_mapSize = 2;
    bool ok = true;
    if (0 == 1) {   // front: start sits at the end of buf0
        d.m_start.m_pNode = &map[0]; d.m_start.m_pFirst = buf0; d.m_start.m_pLast = buf0 + 0x80 / sizeof(SElem);
        d.m_start.m_pCur = d.m_start.m_pLast - 1;
        d.PopFrontAux();
        ok = g_freed == buf0 && d.m_start.m_pNode == &map[1] && d.m_start.m_pFirst == buf1
            && d.m_start.m_pLast == buf1 + 0x80 / sizeof(SElem) && d.m_start.m_pCur == buf1;
    } else {         // back: finish sits at the start of buf1
        d.m_finish.m_pNode = &map[1]; d.m_finish.m_pFirst = buf1; d.m_finish.m_pLast = buf1 + 0x80 / sizeof(SElem);
        d.m_finish.m_pCur = buf1;
        d.PopBackAux();
        ok = g_freed == buf1 && d.m_finish.m_pNode == &map[0] && d.m_finish.m_pFirst == buf0
            && d.m_finish.m_pLast == buf0 + 0x80 / sizeof(SElem) && d.m_finish.m_pCur == buf0 + 0x80 / sizeof(SElem) - 1;
    }
    if (!ok || 0x80 / sizeof(SElem) != (0x80 / sizeof(long))) { std::printf("0059ad35_TEST FAIL\n"); return 1; }
    std::printf("0059ad35_TEST PASS\n");
    return 0;
}