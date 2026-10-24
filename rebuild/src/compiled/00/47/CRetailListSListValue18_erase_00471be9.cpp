// CRetailList_SListValue18::erase(iterator first, iterator last) -- STLport list<T>::erase(first, last) for a 24-byte POD element: unlink and free every node of the range, return `last`
// Leaf: ?erase@CRetailList_SListValue18@@QAE?AU_List_iterator@@U2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471BE9, length 48 bytes, `ret 0Ch`; size-optimised TU (`pop ecx` after free).
// Callees: free @ 0x00BFEA14 (the node allocator's deallocate).
// Caller: the list's operator= @ 0x00471B7x.. (copies the overlap with `rep movsd` x6, then erase(first1, last1) when the
// source runs out: `mov ecx,[ebp-4]` = this, the hidden result pointer and the two iterators -- each built in its slot by
// its user copy constructor -- on the stack; else insert(last1, first2, last2) @ 0x00471C19).
// Shape: STLport 4.x `while (first != last) erase(first++); return last;` with the one-node erase inlined: the old node
// kept in edx, `first` advanced (`mov esi,[esi]`), prev->_M_next = next and next->_M_prev = prev, the trivial element
// destructor, `free(node)`; STLport's list keeps no size, so `this` (ecx) is never read. The iterator result goes through
// the hidden pointer (`mov eax,[esp+0Ch] ; mov [eax],edi`). Under "s" the one-node erase is __forceinline.
// Label correction: the catalog's `_Copy_impl<_List_unchecked_iterator<..CHeroMarriageInfo..>, back_insert_iterator<..>>`
// is propagated from 0x005810AE; the 24-byte element fits CHeroMarriageInfo (0x18) but is unverified -- descriptive names.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* p);           // 0x00BFEA14

struct SListValue18 {                            // 24 bytes, POD (assigned with rep movsd by the caller)
    long m_words[6];
};

struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};
struct _List_node : _List_node_base {
    SListValue18 _M_data;                        // +0x08
};

struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* n) : _M_node(n) { }
    __forceinline _List_iterator(const _List_iterator& o) : _M_node(o._M_node) { }
    __forceinline bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
    __forceinline _List_iterator operator++(int) { _List_iterator tmp = *this; _M_node = _M_node->_M_next; return tmp; }
};

struct CRetailList_SListValue18 {                // STLport list<SListValue18>: the head node pointer only
    _List_node_base* _M_node;
    typedef _List_iterator iterator;
    __forceinline iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node* n = (_List_node*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(n);
        return iterator(next);
    }
    iterator erase(iterator first, iterator last);
};

CRetailList_SListValue18::iterator CRetailList_SListValue18::erase(iterator first, iterator last)
{
    while (first != last)
        erase(first++);
    return last;
}