// Behaviour test for CGameScriptInterface::RemoveAllCutSceneSkippedMessages (0x0089AF30): every type-0x32 event is
// erased from the manager's list (destroyed, node freed), other events stay, and nothing breaks on an empty list.
#include <cstdio>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
struct CMessageEvent {                             // 0x58 bytes, polymorphic
    int m_type; static int s_destroyed;
    virtual ~CMessageEvent() { ++s_destroyed; }
};
struct SMessageNode {                              // list<CMessageEvent> node: next, prev, then the event at +8
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    CMessageEvent m_value;
};
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
    bool operator()(const CMessageEvent* event) const { return event->m_type == Type; }
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
    template<class TFilter> long FindAllMessages(const TFilter& filter, CMessagePtrList& found);
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

struct CWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60
};
struct CGameScriptInterface {
    virtual void v000() {}
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

int CMessageEvent::s_destroyed = 0;
static SMessageNode* NewNode(SMessageNode* head, int type)
{
    SMessageNode* n = (SMessageNode*)malloc(sizeof(SMessageNode));
    new (&n->m_value) CMessageEvent; n->m_value.m_type = type;
    n->m_pNext = head; n->m_pPrev = head->m_pPrev; head->m_pPrev->m_pNext = n; head->m_pPrev = n;
    return n;
}
int main()
{
    SMessageNode* head = (SMessageNode*)malloc(sizeof(SMessageNode)); head->m_pNext = head->m_pPrev = head;
    int types[5] = { 0x32, 7, 0x32, 0x32, 9 };
    for (int i = 0; i < 5; ++i) NewNode(head, types[i]);
    CMessageEventManager manager; manager.World = 0; manager.m_pMessages = head;
    CWorld world; world.PMessageEventManager = &manager;
    CGameScriptInterface gsi; gsi.World = &world;
    gsi.RemoveAllCutSceneSkippedMessages();
    int bad = 0, count = 0;
    for (SMessageNode* n = head->m_pNext; n != head; n = n->m_pNext) { ++count; if (n->m_value.m_type == 0x32) bad = 1; }
    if (count != 2 || CMessageEvent::s_destroyed != 3) bad = 2;
    if (head->m_pNext->m_value.m_type != 7 || head->m_pPrev->m_value.m_type != 9 || head->m_pNext->m_pNext->m_pPrev != head->m_pNext) bad = 3;
    gsi.RemoveAllCutSceneSkippedMessages();
    if (CMessageEvent::s_destroyed != 3) bad = 4;
    if (bad) { std::printf("0089af30_TEST FAIL\n"); return 1; }
    std::printf("0089af30_TEST PASS\n");
    return 0;
}