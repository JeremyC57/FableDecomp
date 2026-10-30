// std::list::list(const list& __x) -- (<std::pair<long, long>>) the copy constructor: an empty list, then insert(begin(), x.begin(), x.end())
// Grade: RELOCATION_MATCH -- a same-TU `__declspec(noinline)` body for _List_base(const allocator&) 0x0053C94A under auto_inline(off) (it never reads the tag): VC7.1 then overlays the empty allocator temporary on the live __x slot ([ebp+0Bh]) and drops the push-ecx frame word, as retail (the "RTM lane gap" label was wrong). Only the copy constructor is graded; the stand-in body allocates and links the sentinel node
// Leaf: ??0?$list@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CB20, 47 bytes, thiscall, `ret 4`, frame-pointer size module.
// Identity (proven): callers NUISystem::CList::SaveState 0x0053852B (copies its structure into a CListState: Ego_r.pdb CListState::Structure is list<std::pair<long,long> >) and 0x0053CB9B; the base constructor 0x0053C94A is bound ??0?$list@U?$pair@JJ@std@@....
// Callees: _List_base<std::pair<long, long>>(const allocator&) 0x0053C94A (sentinel node), list<std::pair<long, long>>::insert<const_iterator>(iterator,
//   const_iterator, const_iterator) 0x0053CB02.
// Shape (family M022, representative 0x0042BF06): `optimize("y", off)` + `optimize("s", on)` (push ebp frame, unaligned
//   entry). The pos iterator has STLport's user copy constructor `_List_iterator(const iterator&)`, so begin() is built in
//   the argument area (`push ecx ; mov eax,esp ; mov [eax],ecx`); const_iterator's copy is implicit (plain pushes).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _List_base(const _Alloc& __a);                        // 0x0053C94A
    _List_node<_Tp>* _M_node;
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) _List_base<_Tp, _Alloc>::_List_base(const _Alloc&)
{
    _List_node<_Tp>* __n = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
    __n->_M_next = __n;
    __n->_M_prev = __n;
    _M_node = __n;
}
#pragma auto_inline(on)
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node); }
    list(const list& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last);   // 0x0053CB02
};
template list<std::pair<long, long> >::list(const list<std::pair<long, long> >&);
}