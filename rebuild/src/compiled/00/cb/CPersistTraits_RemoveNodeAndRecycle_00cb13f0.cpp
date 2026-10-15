// CPersistTraits::RemoveNodeAndRecycle(SNode* node) -- unlink a node from the pooled doubly-linked list, return its payload, and either push the node onto the free list (while the pool is below its cap) or delete it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB13F0, length 107 bytes (+ int3 padding).
// Callees: operator delete @ 0x00BFE9BC.
// The catalogued owner "CPersistTraits" could not be confirmed (no vtable or PDB match; callers 0x00CAFFFA, 0x00CB1903,
// 0x00CB1914 and a jump at 0x00CAEF10); the list layout is read from the body: head +0, tail +4, count +8, free-pool cap
// +0x0c, free-pool count +0x10, free list +0x14; node {prev, next, payload}.
// Shape: a null node returns the null already in eax; the prev/next unlink arms test each neighbour; the payload is
// loaded before the signed pool test (`jge`); recycle-or-delete as an if/else followed by ONE `--m_count; return data;`
// (VC duplicates the tail into both arms; writing the tail twice gives load/inc/dec/store instead of `inc [m] ; dec [m]`); both arms decrement the
// count and return the payload (`ret 4`).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CPersistTraits {
    struct SNode {
        SNode* prev;                               // +0x00
        SNode* next;                               // +0x04
        void*  data;                               // +0x08
    };
    SNode* m_head;                                 // +0x00
    SNode* m_tail;                                 // +0x04
    long   m_count;                                // +0x08
    long   m_maxFree;                              // +0x0c
    long   m_freeCount;                            // +0x10
    SNode* m_freeList;                             // +0x14
    void* RemoveNodeAndRecycle(SNode* node);
};

void* CPersistTraits::RemoveNodeAndRecycle(SNode* node)
{
    if (!node)
        return 0;
    if (node->prev == 0)
        m_head = node->next;
    else
        node->prev->next = node->next;
    if (node->next == 0)
        m_tail = node->prev;
    else
        node->next->prev = node->prev;
    void* data = node->data;
    if (m_freeCount < m_maxFree) {
        node->next = m_freeList;
        m_freeList = node;
        ++m_freeCount;
    } else {
        delete node;
    }
    --m_count;
    return data;
}