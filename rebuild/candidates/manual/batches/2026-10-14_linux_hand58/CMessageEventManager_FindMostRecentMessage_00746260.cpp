// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard> >(const CFilter_Bind2<...>& filter) -- the first event of the message list (+4, newest first) accepted by both bound filters (IsOfType @ 0x006AF1C0, then CanBeSeenOrHeard @ 0x006AF220), null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746260, length 72 bytes.
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0, CMessageEventFilter_CanBeSeenOrHeard::operator() @ 0x006AF220. The bound filter holds POINTERS to its two operands (the landed CFilter_Bind2 model, 0x00910D50); each is re-read from the bind for every pass. Shape: the manager in ebp, the bind in ebx, the node in esi and the event (node + 8) in edi, `ret 4`.
struct CMessageEvent;                              // PDB layout: Type +8, OriginatorUID +0x20, TimeStamp +0x34, Lifespan +0x38, MessageID +0x40, POriginator +0x48 (0x58 bytes)
struct SMessageNode {                              // the STLport-style list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
struct CMessageList {                              // list<CMessageEvent>: one sentinel pointer, no size word (size() walks)
    SMessageNode* m_pHead;
};

struct CMessageEventFilter_IsOfType {
    long Type;
    bool operator()(CMessageEvent* event) const;   // 0x006AF1C0
};
struct CMessageEventFilter_CanBeSeenOrHeard {
    void* m_pThing;
    bool operator()(CMessageEvent* event) const;   // 0x006AF220
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
    bool operator()(CMessageEvent* event) const { return (*m_pFirst)(event) && (*m_pSecond)(event); }
};

struct CWorld;
struct CMessageEventManager {
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    template<class TFilter> CMessageEvent* FindMostRecentMessage(const TFilter& filter)
    {
        for (SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
            CMessageEvent* event = node->GetEvent();   // a local: retail keeps node + 8 in a register and returns it
            if (filter(event))
                return event;
        }
        return 0;
    }
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CBoundFilter_00746260;
template CMessageEvent* CMessageEventManager::FindMostRecentMessage<CBoundFilter_00746260>(const CBoundFilter_00746260& filter);
