#pragma optimize("s", on)
#pragma optimize("y", off)
// CMessageEventManager::RemoveMessage(const CMessageEvent* event) -- erase the node of the message list (+4) that holds the given event (list::erase @ 0x004937CB with the iterator by value), nothing when it is not listed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493B5B, length 48 bytes.
// Callees: list<CMessageEvent>::erase(iterator) @ 0x004937CB (thiscall; the 4-byte iterator has a user copy constructor so it is built in its argument slot -- `push ecx ; mov edx,esp ; mov [edx],eax` -- and the iterator result comes back by hidden pointer into the dead `event` argument slot). Size-optimised module with the frame pointer kept (`push ebp ; mov ebp,esp`): optimize("s") + optimize("y", off). Shape: the list in ecx, its sentinel in edx, the node in eax, `ret 4`.
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

struct CMessageIterator {                          // list<CMessageEvent>::iterator: one node pointer with a user copy constructor
    SMessageNode* m_pNode;
    CMessageIterator(SMessageNode* node) : m_pNode(node) { }
    CMessageIterator(const CMessageIterator& other) : m_pNode(other.m_pNode) { }
};

struct CMessageEventList : CMessageList {
    CMessageIterator erase(CMessageIterator where);   // 0x004937CB
};

struct CWorld;
struct CMessageEventManager {
    CWorld*           World;                       // +0x00
    CMessageEventList Messages;                    // +0x04
    void RemoveMessage(const CMessageEvent* event);
};

void CMessageEventManager::RemoveMessage(const CMessageEvent* event)
{
    for (SMessageNode* node = Messages.m_pHead->m_pNext; node != Messages.m_pHead; node = node->m_pNext) {
        if (node->GetEvent() == event) {
            Messages.erase(CMessageIterator(node));
            return;
        }
    }
}