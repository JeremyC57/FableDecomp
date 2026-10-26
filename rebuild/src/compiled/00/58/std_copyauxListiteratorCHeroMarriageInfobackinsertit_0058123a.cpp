// std::__copy_aux<_List_iterator<CHeroMarriageInfo>, back_insert_iterator>(first, last, result, const __false_type&) -- copy a marriage list into a vector through a back_insert_iterator
// Leaf: __copy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058123A, 44 bytes, `ret 0Ch`.
// Callee: std::__copy(first, last, result, const input_iterator_tag&, ptrdiff_t*) @ 0x005810AE (the list walk that
// calls vector<CHeroMarriageInfo>::push_back 0x00580500 per node). Only caller 0x00581394, in the CTCHeroStats TU.
// Label: the catalog "ConstructString" (CWideString*, long, long) is wrong: the body is STLport 4.x `__copy_aux`
// for non-pointer iterators, `/Gr` `__fastcall` returning the output iterator by value: hidden result pointer in
// ecx; the two list iterators (user copy ctor) and the 4-byte back_insert_iterator (trivially copyable) on the stack;
// the `__false_type` reference would be in edx (unused) -- `ret 0Ch`. It forwards to `__copy` with the iterator
// category temporary, which VC7.1 homes in the high byte of the already pushed `result` slot (`lea edx,[ebp+13h]`),
// and a null `_Distance*`. The list iterators are rebuilt in the argument area (`push ecx ; mov eax,esp ;
// mov [eax],ecx`), the plain output iterator is pushed straight from its slot. Sibling: 0x00581266 is the same
// `__copy_aux` for `const CHeroMarriageInfo*` (first passed through in edx).
// Model: CHeroMarriageInfo modelled locally (24 bytes, PDB members; its copy ctor is out of line).
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

class CHeroMarriageInfo { public:                          // 0x18 bytes (Ego_r.pdb)
    unsigned __int64 WifeUID;                       // +0x00
    unsigned __int64 MaritalHomeUID;                // +0x08
    long             State;                         // +0x10 EMarriageState
    bool             GayMarriage;                   // +0x14
    CHeroMarriageInfo(const CHeroMarriageInfo& other);   // 0x0057A3A2
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);                 // 0x00580500
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
};

typedef _List_iterator<CHeroMarriageInfo>                 CMarriageListIter;
typedef back_insert_iterator<vector<CHeroMarriageInfo> >  CMarriageVectorInserter;

CMarriageVectorInserter __fastcall __copy(CMarriageListIter __first, CMarriageListIter __last,
                                          CMarriageVectorInserter __result, const input_iterator_tag&,
                                          ptrdiff_t*);                                              // 0x005810AE

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, input_iterator_tag(), (ptrdiff_t*)0);
}

template CMarriageVectorInserter __fastcall __copy_aux(CMarriageListIter, CMarriageListIter, CMarriageVectorInserter,
                                                       const __false_type&);

}