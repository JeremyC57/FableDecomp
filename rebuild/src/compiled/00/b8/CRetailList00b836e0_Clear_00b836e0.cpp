// CRetailList_00b836e0::Clear() -- clear a list of counted triangle-list pointers: release each node's pointer (Destroy on the last reference), free the node, relink the sentinel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B836E0, length 72 bytes. Callees: the pointee's virtual slot 1 (+0x04,
// Destroy when its count at +4 reaches zero), free @ 0x00BFEA14.
// The manifest label (CNavigatorQueueManager::FrameUpdate) is propagated over the VC7.1 list-clear bodies (see the
// landed 0x009F9B30 / 0x0075F580 / 0x0070BA00) and wrong here: CNavigatorQueueManager's list holds {long, navigator}
// pairs, while this list's value at node +8 is a CIVCountedPointer (the neighbouring _Dest_val @ 0x00B83500 is
// allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>>); named by effect.
// Shape: the VC7.1 `list::clear` (successor read before the node is destroyed, sentinel relinked to itself at the end)
// with the counted pointer's inline destructor: one load of the object into ecx, the count decremented in place
// (`dec [ecx+4] ; jne`) by the pointee's own Release, the zero store inside the null test.
// Leaf: Clear
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CTriangleList {                              // counted pointee: vptr, then the reference count
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

struct CIVCountedPointer_TriangleList {
    CTriangleList* Object;
    ~CIVCountedPointer_TriangleList()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

struct SNode_00b836e0 {
    SNode_00b836e0*                m_pNext;        // +0x00
    SNode_00b836e0*                m_pPrev;        // +0x04
    CIVCountedPointer_TriangleList m_value;        // +0x08
};

struct CRetailList_00b836e0 {
    SNode_00b836e0* m_pHead;                       // +0x00: the sentinel
    void Clear();
};

void CRetailList_00b836e0::Clear()
{
    SNode_00b836e0* next = m_pHead->m_pNext;
    while (next != m_pHead) {
        SNode_00b836e0* node = next;
        next = next->m_pNext;
        node->m_value.~CIVCountedPointer_TriangleList();
        free(node);
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}