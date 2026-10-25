// CNodePool_00c3c8c0::Init(void* block, long stride, long count) -- make a node pool recycle into a preallocated block: thread the first count-1 nodes of the block (stride bytes apart) together through each node's Link(prev, next), then publish the block as the free chain with `count` free nodes
// Leaf: ?Init@CNodePool_00c3c8c0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C8C0, 101 bytes, __fastcall `ret 0Ch`.
// Library: libpk audio (no PDB/RTTI); descriptive names. The 20-byte pool of the landed PushActive 0x00C3C9B0 /
//   PushFree 0x00C3CA00 (Clear 0x00C3C930 undoes this). Callers: the audio system's setup 0x00C33316 (global pool
//   0x0143BC00 with stride 0x40), 0x00C3E0C8, 0x00C3E144, 0x00C3E1B7, 0x00C3E230, 0x00C3F8C6, 0x00C3F94A. The
//   manifest's "WordWrap_SetupCallbacks" is propagated.
// Callee: the node's Link(prev, next) 0x00C4CED0 (thiscall on the node).
// Shape: a `while (--n)` count-down loop (`dec ecx ; je` entry test, `dec ecx ; jne` back edge); with block, stride,
//   node, next and prev in registers VC spills `this` to the `push ecx` frame word and keeps the counter in the dead
//   block argument slot; the final stores re-read `count` from its slot.
struct CPooledNode_00c3c8c0 {                   // intrusive doubly-linked node
    virtual ~CPooledNode_00c3c8c0();            // slot 0
    CPooledNode_00c3c8c0* m_pPrev;              // +0x04
    CPooledNode_00c3c8c0* m_pNext;              // +0x08
    void Link(CPooledNode_00c3c8c0* prev, CPooledNode_00c3c8c0* next);   // 0x00C4CED0
};

struct CNodePool_00c3c8c0 {                     // 20 bytes: an active chain and a free chain
    void*                 m_pOwner;             // +0x00
    CPooledNode_00c3c8c0* m_pActiveHead;        // +0x04
    CPooledNode_00c3c8c0* m_pFreeHead;          // +0x08
    long                  m_freeCount;          // +0x0c (negative: recycling disabled)
    long                  m_activeCount;        // +0x10
    void Init(void* block, long stride, long count);
};

void CNodePool_00c3c8c0::Init(void* block, long stride, long count)
{
    CPooledNode_00c3c8c0* prev = 0;
    CPooledNode_00c3c8c0* node = (CPooledNode_00c3c8c0*)block;
    CPooledNode_00c3c8c0* next = (CPooledNode_00c3c8c0*)((char*)block + stride);
    long n = count;
    while (--n) {
        node->Link(prev, next);
        prev = node;
        node = next;
        next = (CPooledNode_00c3c8c0*)((char*)next + stride);
    }
    m_pFreeHead = (CPooledNode_00c3c8c0*)block;
    m_freeCount = count;
}