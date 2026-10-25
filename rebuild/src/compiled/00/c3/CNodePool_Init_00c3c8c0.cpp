// CNodePool::Init(void* block, long stride, long count) -- make a node pool recycle into a preallocated block: thread the first count-1 nodes of the block (stride bytes apart) together through each node's Link(prev, next), then publish the block as the free chain with `count` free nodes
// Leaf: ?Init@CNodePool@@QAEXPAXJJ@Z
// Naming (sweep1, 2026-10-25): the address suffix of the class is dropped so the libpk node pool has one name set
//   (CNodePool / CPooledNode) across its rows, as in the landed-error renames GetFree 0x00C3C9A0, PushActive 0x00C3C9B0,
//   PushFree 0x00C3CA00, PopFree 0x00C3CA30, CPooledNode::Link 0x00C4CED0 / Unlink 0x00C4CEF0 (FINDINGS chunk 438).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C8C0, 101 bytes, __fastcall `ret 0Ch`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 20-byte pool of the landed PushActive 0x00C3C9B0 /
//   PushFree 0x00C3CA00 (Clear 0x00C3C930 undoes this). Callers: the audio system's setup 0x00C33316 (global pool
//   0x0143BC00 with stride 0x40), 0x00C3E0C8, 0x00C3E144, 0x00C3E1B7, 0x00C3E230, 0x00C3F8C6, 0x00C3F94A. The
//   manifest's "WordWrap_SetupCallbacks" is propagated.
// Callee: the node's Link(prev, next) 0x00C4CED0 (thiscall on the node).
// Shape: a `while (--n)` count-down loop (`dec ecx ; je` entry test, `dec ecx ; jne` back edge); with block, stride,
//   node, next and prev in registers VC spills `this` to the `push ecx` frame word and keeps the counter in the dead
//   block argument slot; the final stores re-read `count` from its slot.
struct CPooledNode {                   // intrusive doubly-linked node
    virtual ~CPooledNode();            // slot 0
    CPooledNode* m_pPrev;              // +0x04
    CPooledNode* m_pNext;              // +0x08
    void Link(CPooledNode* prev, CPooledNode* next);   // 0x00C4CED0
};

struct CNodePool {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode* m_pActiveHead;        // +0x04
    CPooledNode* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void Init(void* block, long stride, long count);
};

void CNodePool::Init(void* block, long stride, long count)
{
    CPooledNode* prev = 0;
    CPooledNode* node = (CPooledNode*)block;
    CPooledNode* next = (CPooledNode*)((char*)block + stride);
    long n = count;
    while (--n) {
        node->Link(prev, next);
        prev = node;
        node = next;
        next = (CPooledNode*)((char*)next + stride);
    }
    m_pFreeHead = (CPooledNode*)block;
    m_freeCount = count;
}