// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::~CScreenFilterEntry() -- this class's vtable, the entry list at +0x30 cleared (erase from the head's first link, re-link the head, zero the count) and its head node freed, then the CStackEntry base's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00884B60, length 83 bytes.
// Callees: the list's erase @ 0x008834F0 (on the list; the manifest label "LTextTreeWalkThrough::BuildTreeArray" is propagated), free @ 0x00BFEA14. Vtables 0x01278060 =
// CScreenFilterEntry, 0x01277FF0 = CStackEntry (rtti-vtables.tsv). Shape: the list address is formed with `lea esi,[edi+30h]` before the vtable store; the head node is
// re-read from the list for each store; the base destructor's vtable store is last.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SEntryNode_00884b60 {                          // the list node: +4 the first entry link, +8 next, +0xc prev
    long             m_0;
    SEntryNode_00884b60* m_pFirst;                    // +0x04
    SEntryNode_00884b60* m_pNext;                     // +0x08
    SEntryNode_00884b60* m_pPrev;                     // +0x0c
};

struct CRetailEntryList_00884b60 {                    // a retail list: head node and count; clear() erases from the head's first link and re-links the head
    SEntryNode_00884b60* m_pHead;                     // +0x00
    long             m_count;                      // +0x04
    void EraseFrom(SEntryNode_00884b60* node);        // 0x008834F0
    void clear()
    {
        if (m_count) {
            EraseFrom(m_pHead->m_pFirst);
            m_pHead->m_pNext = m_pHead;
            m_pHead->m_pFirst = 0;
            m_pHead->m_pPrev = m_pHead;
            m_count = 0;
        }
    }
    ~CRetailEntryList_00884b60()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
};

namespace NScreenEffect {
struct CScreenEffectPriorityStack {
    struct CStackEntry {                           // vtable 0x01277FF0
        virtual void v00();
        unsigned char _pad[0x2c];                  // +0x04 .. +0x30
        ~CStackEntry() { }
    };
    struct CScreenFilterEntry : CStackEntry {      // vtable 0x01278060
        CRetailEntryList_00884b60 m_entries;       // +0x30
        virtual void v00();
        ~CScreenFilterEntry();
    };
};
}

NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::~CScreenFilterEntry()
{
}