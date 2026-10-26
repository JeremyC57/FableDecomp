// Behaviour test for 0x00644D0E NPlayerGui::CDrawQuestInfo::ClearPrimitivesIfNecessary
#include <cstdio>
static int g_cleared = 0;
static int g_baseCalls = 0;
static int g_destroyed = 0;
static int g_lastNotified = 0;
struct CPrimitive {
    long m_refCount;
    void Destroy() { ++g_destroyed; }
    void OnLastHandleCleared() { ++g_lastNotified; }
};
class CEnginePrimitiveHandle { public:
    CPrimitive* m_p;
    void Clear()
    {
        if ((m_p ? m_p->m_refCount : 0) == 1)
            m_p->OnLastHandleCleared();
        CPrimitive* object = m_p;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            m_p = 0;
        }
        m_p = 0;
        ++g_cleared;
    }
};
struct CMemFunRef {
    void (CEnginePrimitiveHandle::*m_pfn)();
    explicit CMemFunRef(void (CEnginePrimitiveHandle::*pfn)()) : m_pfn(pfn) {}
    void operator()(CEnginePrimitiveHandle& h) const { (h.*m_pfn)(); }
};
CMemFunRef for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRef f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
    void Set(CEnginePrimitiveHandle* f, long n) { m_pFirst = f; m_pLast = f + n; m_pEnd = f + n; }
};
struct CQuestInfoElement {
    int m_clears;
    void Clear() { ++m_clears; }
};
struct node { node* next; node* prev; CQuestInfoElement* m_pObject; void* m_pBlock; };
struct CDrawQuestInfo {
    bool Drawing;
    CRetailHandleVector OraclePrimitiveHandles;
    node* head;
    void BaseClearPrimitivesIfNecessary() { ++g_baseCalls; }
    void ClearPrimitivesIfNecessary()
    {
        if (!Drawing) {
            for (node* n = head->next; n != head; n = n->next) {
                CQuestInfoElement* element = n->m_pObject;
                if (element)
                    element->Clear();
            }
            for_each(OraclePrimitiveHandles.begin(), OraclePrimitiveHandles.end(), CMemFunRef(&CEnginePrimitiveHandle::Clear));
        }
        BaseClearPrimitivesIfNecessary();
    }
};
int main()
{
    int ok = 1;
    CQuestInfoElement e1; e1.m_clears = 0;
    CQuestInfoElement e2; e2.m_clears = 0;
    node sentinel; node n1; node n2; node n3;
    sentinel.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &sentinel;
    n1.m_pObject = &e1; n2.m_pObject = 0; n3.m_pObject = &e2;
    CPrimitive prims[1]; prims[0].m_refCount = 2;
    CEnginePrimitiveHandle h[1]; h[0].m_p = &prims[0];
    CDrawQuestInfo q; q.head = &sentinel; q.OraclePrimitiveHandles.Set(h, 1);
    q.Drawing = true;
    q.ClearPrimitivesIfNecessary();
    ok = e1.m_clears == 0 && g_baseCalls == 1 && g_cleared == 0;
    q.Drawing = false;
    q.ClearPrimitivesIfNecessary();
    ok = ok && e1.m_clears == 1 && e2.m_clears == 1 && g_cleared == 1 && prims[0].m_refCount == 1 && g_baseCalls == 2;
    if (!ok) { std::printf("00644d0e_TEST FAIL\n"); return 1; }
    std::printf("00644d0e_TEST PASS\n");
    return 0;
}