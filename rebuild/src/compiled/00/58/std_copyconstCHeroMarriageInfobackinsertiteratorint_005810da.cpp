// std::__copy<const CHeroMarriageInfo*, back_insert_iterator, int>(first, last, result, tag, int*) -- append [first, last) of a marriage array to a list through a back_insert_iterator
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x005810DA, 58 bytes, `ret 10h`.
// Callee: list<CHeroMarriageInfo>::push_back(const CHeroMarriageInfo&) @ 0x0058056F (insert(end(), x) through
// 0x0057FD7D; its node is malloc(0x20) = links + the 24-byte record built by the copy ctor 0x0057A3A2).
// Only caller 0x00581266 (`copy`/`__copy_aux`, which passes `first` straight through in edx), in the CTCHeroStats
// TU next to the landed vector<CHeroMarriageInfo> helpers (0x00580500 push_back, 0x0057F24E __uninitialized_copy).
// Label: the catalog "LoopConstructClosures" is wrong: the body is STLport 4.x random-access `__copy`, `/Gr`
// `__fastcall` with the iterator returned by value: the hidden result pointer in ecx, `first` in edx, then
// `last`, the 4-byte back_insert_iterator by value, the `random_access_iterator_tag` reference and the null
// `_Distance*` on the stack (`ret 10h`). The count `n = last - first` (`idiv` by 24) is kept in the dead `last`
// slot (`dec dword ptr [ebp+8]`); `*result = *first` is the inline `operator=` calling the container's push_back.
// Model: CHeroMarriageInfo is modelled locally with its PDB members because retail's copy constructor is out of
// line (0x0057A3A2) and the generated engine header cannot declare it.
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CHeroMarriageInfo { public:                          // 0x18 bytes (Ego_r.pdb)
    unsigned __int64 WifeUID;                       // +0x00
    unsigned __int64 MaritalHomeUID;                // +0x08
    long             State;                         // +0x10 EMarriageState
    bool             GayMarriage;                   // +0x14
    CHeroMarriageInfo(const CHeroMarriageInfo& other);   // 0x0057A3A2
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };

template <class _Tp> class list {
public:
    _List_node_base<_Tp>* _M_node;
    void push_back(const _Tp& __x);                 // 0x0058056F
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator& operator=(const CHeroMarriageInfo& __val) { container->push_back(__val); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template back_insert_iterator<list<CHeroMarriageInfo> > __fastcall
__copy(const CHeroMarriageInfo*, const CHeroMarriageInfo*, back_insert_iterator<list<CHeroMarriageInfo> >,
       const random_access_iterator_tag&, int*);

}