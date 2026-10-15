// CMessageEventManager::FindAllMessages<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween> >(const CFilter_Bind2<...>&, CMessagePtrList&) -- append every message accepted by both bound filters to the output list and return its size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006178F0, 128 bytes (padding included).
// Grade: DIFFER(126v127) -- register-save placement only: retail pushes ebx/ebp/esi/edi at entry, ours pushes ebp/edi at entry and defers `push ebx ; push esi` past the empty-list test (with matching pops before the size walk); every other instruction matches. Tried: pointer loop, STLport iterator struct, optimize("s")
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

struct CMessageEvent;
struct SMessageNode {                              // list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
struct CMessageList {
    SMessageNode* m_pHead;
};

struct CMessageEventFilter_IsOfType {
    bool operator()(CMessageEvent* event) const;   // 0x006AF1C0
};
struct CMessageEventFilter_OccuredBetween {
    bool operator()(CMessageEvent* event) const;   // 0x006AF180
};

template <class A, class B> struct CFilter_Bind2 {
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
    SMessagePtrNode* m_pHead;
    void push_back(CMessageEvent* const& value)
    {
        SMessagePtrNode* pos = m_pHead;
        SMessagePtrNode* node = (SMessagePtrNode*)malloc(sizeof(SMessagePtrNode));
        new (&node->m_value) CMessageEvent*(value);
        SMessagePtrNode* prev = pos->m_pPrev;
        node->m_pNext = pos;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        pos->m_pPrev = node;
    }
    long size() const
    {
        long count = 0;
        for (SMessagePtrNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
            ++count;
        return count;
    }
};

struct CWorld;
struct CMessageEventManager {
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
