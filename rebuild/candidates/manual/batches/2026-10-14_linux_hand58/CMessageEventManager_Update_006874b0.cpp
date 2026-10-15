// CMessageEventManager::Update() -- expire the message list (+4): every event with a lifespan (+0x38) whose time stamp (+0x34) plus lifespan is before the current time (0x0049D870) is unlinked, destroyed through its virtual destructor and its node freed (the STLport list::erase inlined); the others are kept
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006874B0, length 84 bytes.
// Callees: GetCurrentTime @ 0x0049D870 (`__fastcall`, no arguments; the label GFGetPreMainMemoryUsed is propagated), CMessageEvent::~CMessageEvent through vtable slot 0 (the scalar deleting destructor with flag 0), free @ 0x00BFEA14 (the STLport malloc allocator). Shape: the manager in ebp, the node in esi and the event in ebx, the successor in edi across the erase, an unsigned time compare, a 16-byte aligned loop head (speed module), `ret`.
struct CMessageEvent {                             // PDB layout (0x58 bytes); only the words this body touches are named
    virtual ~CMessageEvent();                      // slot 0
    long          m_4;
    long          Type;                            // +0x08
    unsigned char _pad0c[0x28];
    long          TimeStamp;                       // +0x34
    unsigned long Lifespan;                        // +0x38
    unsigned char _pad3c[0x1c];
};
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
long __fastcall GetCurrentTime_0049d870();         // 0x0049D870

struct SMessageNode {                              // the STLport-style list node: next, prev, then the event at +8
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    CMessageEvent m_value;                         // +0x08
};
struct CMessageList {                              // list<CMessageEvent>: one sentinel pointer, no size word
    SMessageNode* m_pHead;
    SMessageNode* Erase(SMessageNode* node)        // STLport list::erase: unlink, destroy the value, free the node, return the successor
    {
        SMessageNode* next = node->m_pNext;
        SMessageNode* prev = node->m_pPrev;
        prev->m_pNext = next;
        next->m_pPrev = prev;
        node->m_value.~CMessageEvent();
        free(node);
        return next;
    }
};

struct CWorld;
struct CMessageEventManager {
    CWorld*      World;                            // +0x00
    CMessageList Messages;                         // +0x04
    void Update();
};

void CMessageEventManager::Update()
{
    SMessageNode* node = Messages.m_pHead->m_pNext;
    while (node != Messages.m_pHead) {
        CMessageEvent* event = &node->m_value;
        if (event->Lifespan != 0 && event->TimeStamp + event->Lifespan < (unsigned long)GetCurrentTime_0049d870())
            node = Messages.Erase(node);
        else
            node = node->m_pNext;
    }
}
