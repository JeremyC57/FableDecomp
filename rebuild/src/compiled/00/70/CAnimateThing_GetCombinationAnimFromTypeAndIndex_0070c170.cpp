// CAnimateThing::GetCombinationAnimFromTypeAndIndex(long type, long index) -- the first anim set in either combination list with this type byte and index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C170, length 94 bytes.
// Callees: none.
// Layout (from the bytes): two std::list<CAnimSet> objects whose head node pointers sit at +0x58 and +0x64; the
// node value (CAnimSet, returned by address: `add eax,8`) has a pointer to a signed short index at +0x04 and
// its type byte at +0x50 (node +0x58). `ret 8`: two long arguments (the byte and short are widened to compare).
// Shape: `return &node->value` inside each loop -- the match jumps straight to the shared `add eax,8` tail;
// the first list's head stays in edx, the second's in ecx (this is dead by then).
// VC6-era Dinkumware std::list: a 12-byte object {allocator word, head node, size}; the head's next is begin().
template<class T> struct CRetailList {
    struct SNode { SNode* next; SNode* prev; T value; };
    void* m_allocator;                          // +0x00
    SNode* m_head;                              // +0x04
    unsigned long m_size;                       // +0x08
};

class CAnimSet { public:
    long m_value0;                              // +0x00
    short* m_pIndex;                            // +0x04
    unsigned char _pad[0x48];                   // +0x08 .. +0x4f
    unsigned char m_type;                       // +0x50
};

class CAnimateThing { public:
    unsigned char _pad[0x54];
    CRetailList<CAnimSet> m_combinationAnims;   // +0x54 (head at +0x58)
    CRetailList<CAnimSet> m_combinationAnims2;  // +0x60 (head at +0x64)
    CAnimSet* GetCombinationAnimFromTypeAndIndex(long type, long index);
};

CAnimSet* CAnimateThing::GetCombinationAnimFromTypeAndIndex(long type, long index)
{
    CRetailList<CAnimSet>::SNode* head = m_combinationAnims.m_head;
    CRetailList<CAnimSet>::SNode* node;
    for (node = head->next; node != head; node = node->next) {
        if (node->value.m_type == type && *node->value.m_pIndex == index)
            return &node->value;
    }
    head = m_combinationAnims2.m_head;
    for (node = head->next; node != head; node = node->next) {
        if (node->value.m_type == type && *node->value.m_pIndex == index)
            return &node->value;
    }
    return 0;
}