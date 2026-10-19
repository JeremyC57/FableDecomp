// CGameScriptInterface::RemoveAllCutSceneSkippedMessages() -- collect every "cut scene skipped" message (type 0x32) from the world's message manager, then erase each of them from the manager's message list (destroying the event and freeing its node)
// Leaf: RemoveAllCutSceneSkippedMessages
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089AF30, 182 bytes.
// Callees: malloc @ 0x00BFEA0E (the local pointer list's sentinel), CMessageEventManager::FindAllMessages
//   <CMessageEventFilter_IsOfType> @ 0x00493EAB (landed; filter by const reference, output list by reference), each
//   erased event's virtual destructor (slot 0, `push 0`), free @ 0x00BFEA14 (nodes and the sentinel).
// Layout: CGameScriptInterface World +4 (engine/CGameScriptInterface.h; the interface is modelled locally because the
//   header flattens the vptr), CWorld::PMessageEventManager +0x60 (engine/CWorld.h); CMessageEventManager {World, Messages
//   (list<CMessageEvent>, sentinel pointer) +4} as in the landed FindAllMessages; message nodes hold the 0x58-byte event
//   at +8.
// Shape: the 8-byte frame holds the found list (sentinel pointer) and the filter (type 0x32); the list ctor links the
//   new sentinel to itself before storing it; the removal is an inline manager member (find the node whose event
//   address is the found pointer, unlink with next/prev read into locals first, `event->~CMessageEvent()`, free,
//   stop); the found list's inline destructor clears and frees the sentinel. The sentinel stays in edi across the
//   FindAllMessages call only when that template's body (and push_back's) is compiled earlier in the TU (below).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CMessageEvent {                             // 0x58 bytes, polymorphic
    virtual ~CMessageEvent();
    unsigned char _body[0x54];
};
struct SMessageNode {                              // list<CMessageEvent> node: next, prev, then the event at +8
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    CMessageEvent m_value;
};
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
    bool operator()(const CMessageEvent* event) const;   // 0x006AF1C0
};
struct SMessagePtrNode {
    SMessagePtrNode*     m_pNext;
    SMessagePtrNode*     m_pPrev;
    const CMessageEvent* m_value;                  // +0x08
};
struct CMessagePtrList {                           // STLport list<const CMessageEvent*>: the sentinel pointer
    SMessagePtrNode* m_pHead;
    void push_back(const CMessageEvent* const& event);   // 0x00493D16
    CMessagePtrList()
    {
        SMessagePtrNode* node = (SMessagePtrNode*)malloc(sizeof(SMessagePtrNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
    ~CMessagePtrList()
    {
        clear();
        free(m_pHead);
    }
    void clear()
    {
        SMessagePtrNode* node = m_pHead->m_pNext;
        while (node != m_pHead) {
            SMessagePtrNode* victim = node;
            node = node->m_pNext;
            free(victim);
        }
        m_pHead->m_pNext = m_pHead;
        m_pHead->m_pPrev = m_pHead;
    }
};
struct CWorld;
struct CMessageEventManager {
    CWorld*       World;                           // +0x00
    SMessageNode* m_pMessages;                     // +0x04 list<CMessageEvent> sentinel
    template<class TFilter> long FindAllMessages(const TFilter& filter, CMessagePtrList& found);   // 0x00493EAB
    void RemoveMessage(const CMessageEvent* event)
    {
        SMessageNode* head = m_pMessages;
        for (SMessageNode* node = head->m_pNext; node != head; node = node->m_pNext) {
            if (&node->m_value == event) {
                SMessageNode* next = node->m_pNext;
                SMessageNode* prev = node->m_pPrev;
                prev->m_pNext = next;
                next->m_pPrev = prev;
                node->m_value.~CMessageEvent();
                free(node);
                return;
            }
        }
    }
};
// The callees FindAllMessages (0x00493EAB) and push_back (0x00493D16) are templates compiled in this TU too; their
// bodies (the landed FindAllMessages model, a node-linking push_back) are given here under auto_inline(off) so VC7.1
// knows they leave the found list's sentinel word alone -- retail keeps the sentinel in edi across the call.
#pragma auto_inline(off)
void CMessagePtrList::push_back(const CMessageEvent* const& event)
{
    SMessagePtrNode* node = (SMessagePtrNode*)malloc(sizeof(SMessagePtrNode));
    node->m_value = event;
    SMessagePtrNode* prev = m_pHead->m_pPrev;
    node->m_pNext = m_pHead;
    node->m_pPrev = prev;
    prev->m_pNext = node;
    m_pHead->m_pPrev = node;
}
template<class TFilter> long CMessageEventManager::FindAllMessages(const TFilter& filter, CMessagePtrList& found)
{
    for (SMessageNode* node = m_pMessages->m_pNext; node != m_pMessages; node = node->m_pNext) {
        if (filter(&node->m_value)) {
            const CMessageEvent* event = &node->m_value;
            found.push_back(event);
        }
    }
    long count = 0;
    for (SMessagePtrNode* n = found.m_pHead->m_pNext; n != found.m_pHead; n = n->m_pNext)
        ++count;
    return count;
}
#pragma auto_inline(on)

struct CWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                                 // +0x04
    void RemoveAllCutSceneSkippedMessages();
};

void CGameScriptInterface::RemoveAllCutSceneSkippedMessages()
{
    CMessageEventManager* manager = World->PMessageEventManager;
    CMessagePtrList found;
    if (manager->FindAllMessages(CMessageEventFilter_IsOfType(0x32), found) > 0) {
        for (SMessagePtrNode* it = found.m_pHead->m_pNext; it != found.m_pHead; it = it->m_pNext)
            manager->RemoveMessage(it->m_value);
    }
}