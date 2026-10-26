#pragma optimize("s", on)
#pragma optimize("y", off)
// CMessageEventManager::FindAllMessages<CMessageEventFilter_IsOfType>(const CMessageEventFilter_IsOfType& filter, CMessagePtrList& found) -- append every event of the message list (+4) accepted by the filter (operator() @ 0x006AF1C0) to the output pointer list (push_back @ 0x00493D16), returning how many the output list now holds (an STLport size(): a walk of the list)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493EAB, length 81 bytes.
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0 (label propagated), list<CMessageEvent*>::push_back @ 0x00493D16 (thiscall, the pointer by const reference: the event pointer is spilled to the one frame slot and its address pushed). Size-optimised module with the frame pointer kept (`push ebp ; mov ebp,esp ; push ecx`, `leave`): optimize("s") + optimize("y", off). Shape: the manager in ebx, the node in esi and the event in edi, the count loop at the end, `ret 8`.
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

struct SMessagePtrNode {
    SMessagePtrNode* m_pNext;
    SMessagePtrNode* m_pPrev;
    CMessageEvent*   m_value;                      // +0x08
};
struct CMessagePtrList {                           // list<CMessageEvent*>
    SMessagePtrNode* m_pHead;
    void push_back(CMessageEvent* const& event);   // 0x00493D16
};
inline long Distance(SMessagePtrNode* first, SMessagePtrNode* last)   // STLport size(): distance(begin(), end()) through free functions (no `this` register preference: the list pointer stays in eax)
{
    long count = 0;
    for (; first != last; first = first->m_pNext)
        ++count;
    return count;
}

class CWorld;
class CMessageEventManager { public:
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    template<class TFilter> long FindAllMessages(const TFilter& filter, CMessagePtrList& found)
    {
        for (SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
            if (filter(node->GetEvent()))
                found.push_back(node->GetEvent());
        }
        return Distance(found.m_pHead->m_pNext, found.m_pHead);
    }
};

template long CMessageEventManager::FindAllMessages<CMessageEventFilter_IsOfType>(const CMessageEventFilter_IsOfType& filter, CMessagePtrList& found);