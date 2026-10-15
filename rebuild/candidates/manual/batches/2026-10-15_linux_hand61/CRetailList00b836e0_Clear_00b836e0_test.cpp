#include <cstdio>
#include <cstdlib>
// Behaviour test for CRetailList_00b836e0::Clear @ 0x00B836E0
static int g_destroyed = 0;
class CTriangleList {
public:
    CTriangleList(long count) : RefCount(count) {}
    virtual ~CTriangleList() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CIVCountedPointer_TriangleList {
    CTriangleList* Object;
    ~CIVCountedPointer_TriangleList() { if (Object) { Object->Release(); Object = 0; } }
};
struct SNode_00b836e0 { SNode_00b836e0* m_pNext; SNode_00b836e0* m_pPrev; CIVCountedPointer_TriangleList m_value; };
struct CRetailList_00b836e0 {
    SNode_00b836e0* m_pHead;
    void Clear()
    {
        SNode_00b836e0* next = m_pHead->m_pNext;
        while (next != m_pHead) {
            SNode_00b836e0* node = next;
            next = next->m_pNext;
            node->m_value.~CIVCountedPointer_TriangleList();
            std::free(node);
        }
        m_pHead->m_pNext = m_pHead;
        m_pHead->m_pPrev = m_pHead;
    }
};
int main()
{
    CTriangleList last(1), shared(2);
    CRetailList_00b836e0 list;
    list.m_pHead = (SNode_00b836e0*)std::malloc(sizeof(SNode_00b836e0));
    list.m_pHead->m_pNext = list.m_pHead; list.m_pHead->m_pPrev = list.m_pHead;
    CTriangleList* values[3] = { &last, &shared, 0 };
    for (int i = 0; i < 3; ++i) {
        SNode_00b836e0* n = (SNode_00b836e0*)std::malloc(sizeof(SNode_00b836e0));
        n->m_value.Object = values[i];
        n->m_pPrev = list.m_pHead->m_pPrev; n->m_pNext = list.m_pHead;
        list.m_pHead->m_pPrev->m_pNext = n; list.m_pHead->m_pPrev = n;
    }
    list.Clear();
    if (g_destroyed != 1 || last.RefCount != 0 || shared.RefCount != 1 || list.m_pHead->m_pNext != list.m_pHead || list.m_pHead->m_pPrev != list.m_pHead) { std::printf("00b836e0_TEST FAIL\n"); return 1; }
    list.Clear();
    std::free(list.m_pHead);
    std::printf("00b836e0_TEST PASS\n");
    return 0;
}
