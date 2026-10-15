// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_HasExtraData,CMessageEventFilter_OccuredBetween> >(const CFilter_Bind3<...>& filter) -- the first event of the message list (+4, newest first) accepted by all three bound filters (IsOfType @ 0x006AF1C0, HasExtraData @ 0x006AF200, OccuredBetween @ 0x006AF1E0), null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A5B0, length 85 bytes.
// Callees: 0x006AF1C0, 0x006AF200, 0x006AF1E0 (the IsOriginatedByThing labels on the last two are propagated over byte-identical bodies). The bound filter holds POINTERS to its three operands (the landed CFilter_Bind3 model, 0x00910D50); each is re-read from the bind for every pass. Shape: the manager in ebp, the bind in ebx, the node in edi and the event (node + 8) in esi, `ret 4`.
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
struct CMessageEventFilter_HasExtraData {
    bool operator()(CMessageEvent* event) const;   // 0x006AF200
};
struct CMessageEventFilter_OccuredBetween {
    long m_first;
    long m_last;
    bool operator()(CMessageEvent* event) const;   // 0x006AF1E0
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    bool operator()(CMessageEvent* event) const { return (*m_pFirst)(event) && (*m_pSecond)(event) && (*m_pThird)(event); }
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

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_OccuredBetween> CBoundFilter_0091a5b0;
template CMessageEvent* CMessageEventManager::FindMostRecentMessage<CBoundFilter_0091a5b0>(const CBoundFilter_0091a5b0& filter);
