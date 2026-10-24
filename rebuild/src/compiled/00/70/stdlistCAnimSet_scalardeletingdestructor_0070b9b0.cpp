// std::list<CAnimSet>::`scalar deleting destructor'(unsigned int flags) -- non-virtual: clear the animation-set list, free its sentinel node, then the flag-tested operator delete
// Leaf: ??_G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B9B0, length 44 bytes (+ int3 padding). __fastcall (this in ecx), `ret 4`.
// Callees: list<CAnimSet>::clear() @ 0x0070B8A0 (out of line; its manifest label "CNavigatorQueueManager::FrameUpdate" is
//          propagated), free (CRT import thunk) @ 0x00BFEA14 (the sentinel), operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CNavigatorQueueManager_dtor" is propagated. 0x0070B8A0 is the clear of a list whose 0x5C-byte
//   nodes hold a retail CAnimSet (84 bytes: UpdateMode, then the Anims / InternalAlphas vectors whose buffers it frees,
//   node +0x0C / +0x18); it is called from the CAnimateThing TU (ClearQueues 0x0070D510 and others). The object here is
//   the bare 4-byte STLport list (sentinel pointer at +0) -- the same body as Ego_r.pdb CAnimateThing::CDelayedAnimation,
//   whose only member is that list. No callers (an unreferenced COMDAT).
// Shape: STLport 4.x `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with the allocator's
//   `if (p != 0)` free, then the scalar deleting destructor's `test byte ptr [esp+8],1` operator delete; the class is not
//   polymorphic, so the non-virtual `??_G` comes from a `delete` helper (the landed 0x0070B830 / CBufferOwner idiom).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14 (import thunk)

struct SAnimSetNode;                                // STLport _List_node<CAnimSet> (0x5C bytes)

template <class _Tp> struct allocator {            // std::allocator (empty)
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> struct _STLP_alloc_proxy : _Alloc {
    _Value _M_data;
};

struct CAnimSetList {                               // list<CAnimSet> (retail 4 bytes)
    _STLP_alloc_proxy<SAnimSetNode*, allocator<SAnimSetNode> > _M_node;
    void clear();                                   // 0x0070B8A0
    ~CAnimSetList()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CAnimSetList(CAnimSetList* list)
{
    delete list;
}