// _STL::merge(CAnimSetIterator* first1, CAnimSetIterator* last1, CAnimSetIterator* first2, CAnimSetIterator* last2, CAnimSetIterator* result, CAnimSetItPriorityLessThan comp) -- SGI merge of two sorted runs of list<CAnimSet> const iterators by the anim set's Priority (the second run's iterator goes first only when its priority is strictly lower), then both tails copied with the counted copy loop
// Leaf: merge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710310, 114 bytes, `ret 10h` (__fastcall: first1 in ecx, last1 in edx,
// first2, last2, result and the empty comparator's slot on the stack -- the SGI/STLport algorithm TUs are /Gr). No callees.
// Element: a 4-byte list<CAnimSet>::const_iterator (the node pointer); the comparator reads `[node+54h]` = the node's
// value at +8 plus CAnimSet::Priority at +0x4C (Ego_r.pdb's +0x54 sits 8 bytes high: its two vectors are 16 bytes, retail
// 12). Same element/comparator model as the matched insertion sort 0x00711190 (chunk 356).
// Shape: `while (first1 != last1 && first2 != last2) { if (comp(*first2, *first1)) *result = *first2++; else *result =
// *first1++; ++result; } return copy(first2, last2, copy(first1, last1, result));` -- copy() is STLport's counted
// `__copy(first, last, result, random_access_iterator_tag, Distance*)`, both inlined; last2 is re-read from its argument
// slot each iteration (VC7.1 runs out of registers with ebp holding the priority).
// Label: catalogued as Dinkumware `_Merge<_List_const_iterator<_List_val<CAnimSet> >*, ..., CAnimSetItPriorityLessThan>`.
struct CAnimSet {                                 // retail layout (12-byte vectors)
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

struct CAnimSetItPriorityLessThan {               // empty functor, passed in a 4-byte slot
    bool operator()(const CAnimSetIterator& a, const CAnimSetIterator& b) const { return (*a).Priority < (*b).Priority; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

inline CAnimSetIterator* __copy(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

inline CAnimSetIterator* copy(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

CAnimSetIterator* __fastcall merge(CAnimSetIterator* first1, CAnimSetIterator* last1, CAnimSetIterator* first2, CAnimSetIterator* last2,
                                   CAnimSetIterator* result, CAnimSetItPriorityLessThan comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}