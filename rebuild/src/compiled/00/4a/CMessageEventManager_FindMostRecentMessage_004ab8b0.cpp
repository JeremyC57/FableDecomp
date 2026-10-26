// CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType>(const CMessageEventFilter_IsOfType& filter) -- the first event of the message list (+4, newest first) accepted by the filter (operator() @ 0x006AF1C0), null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB8B0, length 57 bytes.
// Callees: CMessageEventFilter_IsOfType::operator()(CMessageEvent*) @ 0x006AF1C0 (the label CMessageEventFilter_HasExtraData is propagated over the byte-identical body). PDB: CMessageEventManager = { CWorld* World; list<CMessageEvent> Messages (+4) }. Shape: the manager in ebx, the filter in ebp, the node in esi and the event (node + 8) in edi, the sentinel re-read for every pass (the filter may alias), `ret 4`.
class CMessageEvent;                              // PDB layout: Type +8, OriginatorUID +0x20, TimeStamp +0x34, Lifespan +0x38, MessageID +0x40, POriginator +0x48 (0x58 bytes)
struct SMessageNode {                              // the STLport-style list node: next, prev, then the event at +8
    SMessageNode*  m_pNext;
    SMessageNode*  m_pPrev;
    unsigned char  m_value[0x58];                  // +0x08 CMessageEvent
    CMessageEvent* GetEvent() { return (CMessageEvent*)m_value; }
};
struct CMessageList {                              // list<CMessageEvent>: one sentinel pointer, no size word (size() walks)
    SMessageNode* m_pHead;
};

class CMessageEventFilter_IsOfType { public:
    long Type;
    bool operator()(CMessageEvent* event) const;   // 0x006AF1C0
};

class CWorld;
class CMessageEventManager { public:
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

template CMessageEvent* CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType>(const CMessageEventFilter_IsOfType& filter);