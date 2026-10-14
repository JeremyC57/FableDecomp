// CDynamicObjectHeap<56,16>::Allocate(unsigned long) -- pops the free-list head, growing the heap when it is empty and growth is allowed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B49660, length 44 bytes.
// Callees: CDynamicObjectHeap<56,16>::CreateMoreBlocks @ 0x00B49010 (manifest, thiscall on `this`).
// The manifest label "CAIStateGroupBase::IsStopping" is propagated and rejected: the body is an
// object-pool allocator -- free-list head at +0x14, live count at +0x04, the can-grow flag at
// +0x39c. The size argument is unused (`ret 4`). Shape: empty free list -> if not growable
// return 0, else refill (written `if (grow) Refill(); else return 0;` -- the negated form hoists the
// failure return above the call); then bump the count, unlink the head (`inc dword ptr`, re-read head,
// store head->next) and return the node.
struct SFreeNode { SFreeNode* next; };

struct CDynamicObjectHeap_56_16 {
    void*         __vftable;                // +0x00
    unsigned long m_allocated;              // +0x04
    unsigned char _pad0[0x0c];              // +0x08..+0x14
    SFreeNode*    m_pFreeList;              // +0x14
    unsigned char _pad1[0x384];             // +0x18..+0x39c
    bool          m_bCanGrow;               // +0x39c
    void CreateMoreBlocks();                // 0x00B49010
    void* Allocate(unsigned long size);
};

void* CDynamicObjectHeap_56_16::Allocate(unsigned long size)
{
    if (m_pFreeList == 0) {
        if (m_bCanGrow)
            CreateMoreBlocks();
        else
            return 0;
    }
    ++m_allocated;
    SFreeNode* node = m_pFreeList;
    m_pFreeList = node->next;
    return node;
}
