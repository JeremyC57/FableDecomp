// CTCAnimatingObject::ClearQueuedAnims() -- empties the queued-animation list (frees every node, then re-links the sentinel to itself)
// Leaf: ClearQueuedAnims
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8BE0, 50 bytes.
// Callees: free @ 0x00BFEA14 (the STLport node allocator's deallocate).
// Label: the chunk label "CParticleSystem::ClearAllDecals" is propagated; the only caller is
// CTCAnimatingObject::PlayAnimation (0x007F90B0, `mov ecx,esi ; call`), and +0x14 is CTCAnimatingObject's
// QueuedAnims (list<CAnimInit>, PDB +0x14), so this is that TC's inlined `QueuedAnims.clear()`. Leaf named by role.
// Shape: STLport `list::clear` for a trivially destructible element: `tmp = cur ; cur = cur->next ; free(tmp)`
// (the loop variable advances before the free), the sentinel re-read from the member after every free and for
// both final stores (`next = prev = sentinel`).
extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {              // STLport list: one word, the sentinel node
    SListNode<T>* _M_node;
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

struct CAnimInit {                                   // PDB 44 bytes, all scalars: trivially destructible
    long          UpdateMode;
    unsigned char Type;
    bool          Looping;
    long          Anim;
    float         Speed;
    float         Alpha;
    float         LastAlpha;
    long          Frame[2];                              // CAnimFrameInfo
    unsigned long Flags;
    unsigned long TypeFlags;
    long          Priority;
};

struct CTCAnimatingObject {
    void*                         m_vtable;                  // +0x00
    void*                         m_pThing;                  // +0x04
    unsigned char                 m_pad08[0x04];
    bool                          Animating;                 // +0x0c
    long                          CurrentAnimationPriority;  // +0x10
    CRetailList<CAnimInit>        QueuedAnims;               // +0x14
    void ClearQueuedAnims();
};

void CTCAnimatingObject::ClearQueuedAnims()
{
    QueuedAnims.clear();
}
