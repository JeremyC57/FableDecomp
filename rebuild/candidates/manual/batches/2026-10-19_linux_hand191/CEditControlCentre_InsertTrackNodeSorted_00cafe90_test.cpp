// Behaviour test for 0x00CAFE90 (catalog CEditControlCentre::InsertTrackNodeSorted): the item is add-ref'd under the
// lock and inserted before the first later entry of the list its flag selects (tail when none), then Refresh runs.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
static int g_locks = 0, g_unlocks = 0;
class CCritSec { public: void Lock() { ++g_locks; } void Unlock() { ++g_unlocks; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) { m_pLock = p; m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
typedef void* POSITION;
struct CScheduledItem { long refs; i64 Time; long UseSecondList; unsigned long AddRef() { return ++refs; } };
struct SNode { SNode* next; SNode* prev; CScheduledItem* obj; };
template <class OBJECT> class CGenericList {
public:
    SNode* m_pFirst; SNode* m_pLast; SNode pool[8]; int used;
    CGenericList() : m_pFirst(0), m_pLast(0), used(0) {}
    POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    OBJECT* Get(POSITION pos) const { return pos ? ((SNode*)pos)->obj : 0; }
    OBJECT* GetNext(POSITION& rp) const { SNode* n = (SNode*)rp; rp = n->next; return n->obj; }
    POSITION AddTail(OBJECT* o) { SNode* n = &pool[used++]; n->obj = o; n->next = 0; n->prev = m_pLast; if (m_pLast) m_pLast->next = n; else m_pFirst = n; m_pLast = n; return n; }
    POSITION AddBefore(POSITION pos, OBJECT* o)
    {
        SNode* at = (SNode*)pos; SNode* n = &pool[used++]; n->obj = o; n->next = at; n->prev = at->prev;
        if (at->prev) at->prev->next = n; else m_pFirst = n; at->prev = n; return n;
    }
};
class CEditControlCentre {
public:
    CCritSec m_Lock; CGenericList<CScheduledItem> m_FirstList; CGenericList<CScheduledItem> m_SecondList; int refreshes;
    void Refresh() { ++refreshes; }
    long InsertTrackNodeSorted(CScheduledItem* item);
};
long CEditControlCentre::InsertTrackNodeSorted(CScheduledItem* item)
{
    CAutoLock lock(&m_Lock);
    item->AddRef();
    CGenericList<CScheduledItem>* list = item->UseSecondList ? &m_SecondList : &m_FirstList;
    POSITION pos = list->GetHeadPosition();
    while (pos) {
        if (list->Get(pos)->Time > item->Time)
            break;
        list->GetNext(pos);
    }
    if (pos == 0)
        list->AddTail(item);
    else
        list->AddBefore(pos, item);
    Refresh();
    return 0;
}
int main()
{
    CEditControlCentre c; c.refreshes = 0;
    CScheduledItem a = { 0, 30, 0 }, b = { 0, 10, 0 }, d = { 0, 20, 0 }, e = { 0, 20, 0 }, p = { 0, 5, 1 };
    int bad = 0;
    if (c.InsertTrackNodeSorted(&a) != 0) bad = 1;
    c.InsertTrackNodeSorted(&b); c.InsertTrackNodeSorted(&d); c.InsertTrackNodeSorted(&e); c.InsertTrackNodeSorted(&p);
    SNode* n = c.m_FirstList.m_pFirst;
    if (n->obj != &b || n->next->obj != &d || n->next->next->obj != &e || n->next->next->next->obj != &a) bad = 2;   // equal times keep insertion order
    if (c.m_SecondList.m_pFirst->obj != &p || c.m_SecondList.used != 1) bad = 3;
    if (a.refs != 1 || p.refs != 1 || g_locks != 5 || g_unlocks != 5 || c.refreshes != 5) bad = 4;
    if (bad) { std::printf("00cafe90_TEST FAIL\n"); return 1; }
    std::printf("00cafe90_TEST PASS\n");
    return 0;
}
