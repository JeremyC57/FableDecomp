// Behaviour test for 006178f0: src/006178f0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CMessageEventManager::FindAllMessages<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween> >(const CFilter_Bind2<...>&, CMessagePtrList&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CMessageEventManager::FindAllMessages<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween> >(const CFilter_Bind2<...>&, CMessagePtrList&) -- append every message accepted by both bound filters to the output list and return its size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006178F0, 128 bytes (padding included).
// Leaf: FindAllMessages
// Label correction: the catalog's "FindMostRecentMessage<CFilter_Bind3<...>>" is a propagated label:
// this body collects every match into the list argument and returns the list's size (the landed
// FindAllMessages<IsOfType> 0x00493EAB is the size-optimised single-filter sibling), and only two
// bound filters are called: [filter+0] with 0x006AF1C0 (IsOfType) and [filter+4] with 0x006AF180
// (OccuredBetween).
// Shape: the manager is spilled (`push ecx` slot) and the list end re-read from it each step; the
// event is passed by address (node + 8); the bind's operator() is the `&&` of the two filters; the
// output push_back is STLport's inline insert(end(), x): node malloc'd (0x00BFEA0E, 12 bytes),
// placement-constructed with the null test, linked before the sentinel; size() walks the list.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* where) { return where; }

class CMessageEvent;
struct SMessageNode {                              // list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
struct CMessageList {
    SMessageNode* m_pHead;
};

class CMessageEventFilter_IsOfType { public:
    bool operator()(CMessageEvent* event) const;   // 0x006AF1C0
};
class CMessageEventFilter_OccuredBetween { public:
    bool operator()(CMessageEvent* event) const;   // 0x006AF180
};

template <class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    bool operator()(CMessageEvent* event) const { return (*m_pFirst)(event) && (*m_pSecond)(event); }
};

struct SMessagePtrNode {
    SMessagePtrNode* m_pNext;
    SMessagePtrNode* m_pPrev;
    CMessageEvent*   m_value;                      // +0x08
};
struct CMessagePtrList {                           // STLport list<CMessageEvent*>
    struct iterator {
        SMessagePtrNode* m_node;
        iterator(SMessagePtrNode* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
        iterator& operator++() { m_node = m_node->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    SMessagePtrNode* m_pHead;
    iterator begin() const { return iterator(m_pHead->m_pNext); }
    iterator end() const { return iterator(m_pHead); }
    SMessagePtrNode* create_node(CMessageEvent* const& value)
    {
        SMessagePtrNode* node = (SMessagePtrNode*)malloc(sizeof(SMessagePtrNode));
        new (&node->m_value) CMessageEvent*(value);
        return node;
    }
    void insert(iterator position, CMessageEvent* const& value)
    {
        SMessagePtrNode* node = create_node(value);
        SMessagePtrNode* pos = position.m_node;
        SMessagePtrNode* prev = pos->m_pPrev;
        node->m_pNext = pos;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        pos->m_pPrev = node;
    }
    void push_back(CMessageEvent* const& value) { insert(end(), value); }
    long size() const
    {
        long count = 0;
        for (iterator it = begin(); it != end(); ++it)
            ++count;
        return count;
    }
};

class CWorld;
class CMessageEventManager { public:
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    template <class TFilter> long FindAllMessages(const TFilter& filter, CMessagePtrList& found)
    {
        for (SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
            if (filter(node->GetEvent()))
                found.push_back(node->GetEvent());
        }
        return found.size();
    }
};

template long CMessageEventManager::FindAllMessages<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> >(
    const CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>& filter, CMessagePtrList& found);

// ---- stubs and driver ----

bool CMessageEventFilter_IsOfType::operator()(CMessageEvent* event) const { return *(long*)event == 7; }
bool CMessageEventFilter_OccuredBetween::operator()(CMessageEvent* event) const { long f = ((long*)event)[1]; return f >= 10 && f <= 20; }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> TFilter;
static SMessageNode g_nodes[5];
int main()
{
    SMessageNode head; head.m_pNext = &head; head.m_pPrev = &head;
    long types[4] = { 7, 7, 6, 7 }, frames[4] = { 15, 30, 15, 10 };
    for (int i = 0; i < 4; ++i) {
        SMessageNode* n = &g_nodes[i]; ((long*)n->m_value)[0] = types[i]; ((long*)n->m_value)[1] = frames[i];
        n->m_pNext = &head; n->m_pPrev = head.m_pPrev; head.m_pPrev->m_pNext = n; head.m_pPrev = n;
    }
    CMessageEventManager mgr; mgr.World = 0; mgr.Messages.m_pHead = &head;
    SMessagePtrNode outHead; outHead.m_pNext = &outHead; outHead.m_pPrev = &outHead;
    CMessagePtrList out; out.m_pHead = &outHead;
    CMessageEventFilter_IsOfType f1; CMessageEventFilter_OccuredBetween f2; TFilter filter; filter.m_pFirst = &f1; filter.m_pSecond = &f2;
    long n = mgr.FindAllMessages(filter, out);
    CHECK(n == 2);
    CHECK(outHead.m_pNext->m_value == g_nodes[0].GetEvent()); CHECK(outHead.m_pNext->m_pNext->m_value == g_nodes[3].GetEvent());
    CHECK(outHead.m_pPrev->m_pNext == &outHead);
    // appends to a non-empty list and counts the whole list
    n = mgr.FindAllMessages(filter, out); CHECK(n == 4);
    SMessageNode empty; empty.m_pNext = &empty; empty.m_pPrev = &empty; mgr.Messages.m_pHead = &empty;
    CHECK(mgr.FindAllMessages(filter, out) == 4);
    if (fails == 0) printf("006178f0_TEST PASS\n"); else printf("006178f0_TEST FAIL\n");
    return fails;
}