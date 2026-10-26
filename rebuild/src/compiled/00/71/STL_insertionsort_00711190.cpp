// std::__insertion_sort(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetItPriorityLessThan comp) -- STLport insertion sort of a vector of list<CAnimSet> const iterators by the anim set's Priority, everything inlined: a lower priority shifts the run up one slot and goes first, anything else sinks with the unguarded linear insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711190, 153 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// comparator slot on the stack -- the SGI/STLport algorithm TUs are /Gr). No callees: __linear_insert, copy_backward /
// __copy_backward (counted loop) and __unguarded_linear_insert are all inlined.
// Element: a 4-byte list<CAnimSet>::const_iterator (the node pointer). The comparator reads `[node+54h]` = the node's
// value at +8 plus CAnimSet::Priority at +0x4C (Ego_r.pdb puts Priority at +0x54, but its two 16-byte vectors are 12
// bytes in retail, GOTCHAS wave 23 -- the PDB offsets after them sit 8 bytes high here).
// Label: catalogued as `_Insertion_sort1<_List_const_iterator<_List_val<CAnimSet> >*, CAnimSetItPriorityLessThan>`
// (Dinkumware name); the code is STLport's `__insertion_sort`.
// Shape: the count of copy_backward is `(i - first) >> 2` from the frame constant `-4 - first` (strength-reduced), the
// moved-down copy indexes `[edi+eax-4]`; the unguarded loop re-reads the value's priority (`mov ebp,[ecx+54h]`) after
// each store because the iterator store may alias. VC7.1 leaves the helpers out of line unless they are __forceinline.
class CAnimSet { public:                                 // retail layout (12-byte vectors)
    unsigned char _before[0x4c];                  // UpdateMode, Anims, InternalAlphas, ... AnimDelay
    long Priority;                                // +0x4C
    unsigned char _after[0x0c];                   // Type, the flag bits, ...
};

struct SAnimSetNode {                             // list<CAnimSet> node
    SAnimSetNode* _M_next;                        // +0x00
    SAnimSetNode* _M_prev;                        // +0x04
    CAnimSet      _M_data;                        // +0x08
};

struct CAnimSetIterator {                         // list<CAnimSet>::const_iterator
    SAnimSetNode* _M_node;
    const CAnimSet& operator*() const { return _M_node->_M_data; }
};

class CAnimSetItPriorityLessThan { public:               // empty functor, passed in a 4-byte slot
    bool operator()(const CAnimSetIterator& a, const CAnimSetIterator& b) const { return (*a).Priority < (*b).Priority; }
};

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

__forceinline CAnimSetIterator* __copy_backward(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result,
                                                const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

__forceinline CAnimSetIterator* copy_backward(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

__forceinline void __unguarded_linear_insert(CAnimSetIterator* last, CAnimSetIterator val, CAnimSetItPriorityLessThan comp)
{
    CAnimSetIterator* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

__forceinline void __linear_insert(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator val, CAnimSetItPriorityLessThan comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

void __fastcall __insertion_sort(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetItPriorityLessThan comp)
{
    if (first == last)
        return;
    for (CAnimSetIterator* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}