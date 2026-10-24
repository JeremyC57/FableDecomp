// _global::Copy_ListToBackInserter_CHeroMarriageInfo_005810ae(_List_iterator first, _List_iterator last, back_insert_iterator<vector<CHeroMarriageInfo>> result, const input_iterator_tag&, int*) -- STLport __copy (input iterators): push_back every CHeroMarriageInfo of a list range into a vector, return the back inserter
// Leaf: Copy_ListToBackInserter_CHeroMarriageInfo_005810ae
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005810AE, length 44 bytes, `ret 10h`; size-optimised TU.
// Callees: vector<CHeroMarriageInfo>::push_back(const CHeroMarriageInfo&) @ 0x00580500 (landed, 24-byte elements).
// Caller: copy(first, last, back_inserter(v)) @ 0x0058123A -- hidden result pointer in ecx, the tag reference in edx
// (`lea edx,[ebp+13h]`, the first register-eligible argument after the hidden pointer), then on the stack the two list
// iterators (each built in its slot by its user copy constructor: `mov eax,esp ; mov [eax],ecx`), the back inserter
// (pushed as a plain word) and the null distance pointer.
// Shape: STLport 4.x `__copy(first, last, result, const input_iterator_tag&, _Distance*)`:
// `for (; first != last; ++first, ++result) *result = *first; return result;` -- the inserter's assignment is
// container->push_back(node->_M_data) (`lea eax,[esi+8] ; push eax ; mov ecx,ebx ; call`), the list step node = node->_M_next,
// `last` compared from its stack slot; the by-value back_insert_iterator (non-POD: user constructor) is returned through
// the hidden pointer (`mov [edi],ebx ; mov eax,edi`). Under "s" the iterator members are __forceinline.
// Label: the catalog's `_Copy_impl<_List_unchecked_iterator<..CHeroMarriageInfo..>, back_insert_iterator<vector<
// CHeroMarriageInfo>>>` names the instantiation (Dinkumware spelling of the STLport __copy).
#pragma optimize("s", on)

#include "engine/CHeroMarriageInfo.h"            // PDB layout, 0x18 bytes (only passed by reference here)

struct CHeroMarriageInfoVector {                 // STLport vector<CHeroMarriageInfo>
    CHeroMarriageInfo* _M_start;
    CHeroMarriageInfo* _M_finish;
    CHeroMarriageInfo* _M_end_of_storage;
    void push_back(const CHeroMarriageInfo& x);  // 0x00580500
};

struct _List_node {
    _List_node*       _M_next;                   // +0x00
    _List_node*       _M_prev;                   // +0x04
    CHeroMarriageInfo _M_data;                   // +0x08
};

struct _List_iterator {
    _List_node* _M_node;
    __forceinline _List_iterator(const _List_iterator& o) : _M_node(o._M_node) { }
    __forceinline CHeroMarriageInfo& operator*() const { return _M_node->_M_data; }
    __forceinline _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    __forceinline bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};

struct back_insert_iterator {
    CHeroMarriageInfoVector* container;
    explicit back_insert_iterator(CHeroMarriageInfoVector& x) : container(&x) { }
    __forceinline back_insert_iterator& operator=(const CHeroMarriageInfo& value) { container->push_back(value); return *this; }
    __forceinline back_insert_iterator& operator*() { return *this; }
    __forceinline back_insert_iterator& operator++() { return *this; }
};

struct input_iterator_tag { input_iterator_tag() { } };

back_insert_iterator __fastcall Copy_ListToBackInserter_CHeroMarriageInfo_005810ae(_List_iterator first, _List_iterator last, back_insert_iterator result, const input_iterator_tag&, int*)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}