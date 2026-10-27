// std::vector<std::map<unsigned long, NUISystem::CUIState>>::erase(std::map<unsigned long, NUISystem::CUIState>* __position) -- remove one element: shift the tail down, destroy the old last slot
// Leaf: ?erase@?$vector@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@V?$allocator@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@@2@@std@@QAEPAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@2@PAV32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CD05, 52 bytes, thiscall, `ret 4` (frame-pointer module).
// Identity (probable): its __copy 0x0053CC38 is bound ??$__copy@PAV?$map@KVCUIState@NUISystem@@... (12-byte stride), the destructor 0x0042B755 is that map's tree destructor, and its only caller is NUISystem::CList::RemoveChild 0x0053B321 (Ego_r.pdb CList has vector<map<unsigned long, NUISystem::CUIState> >).
// Callees: std::__copy<T*, T*, int> 0x0053CC38 (`__fastcall`), the element destructor 0x0042B755.
// Shape (family M123, representative 0x0053CD05): STLport 4.6 `if (__position + 1 != end()) __copy_ptrs(__position + 1,
//   _M_finish, __position, _TrivialAss()); --_M_finish; _Destroy(_M_finish); return __position;` -- __copy out of line with
//   the destructor called directly (the qualified `_Tp::~_Tp()`: under /Os VC7.1 routes an unqualified `p->~T()`
//   through the class's `??_G` with flags 0, which retail does not do here); __copy's empty tag homed in the top byte of the position slot (`optimize("y", off)` + `optimize("s", on)`) (`lea eax,[ebp+0Bh]`) and a null distance pointer.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
namespace NUISystem { class CUIState; }
namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    ~_Rb_tree();                                          // 0x0042B755
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    map& operator=(const map& __x);
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};
}
namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->_Tp::~_Tp(); }   // qualified: a direct destructor call
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator end() { return _M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template vector<std::map<unsigned long, NUISystem::CUIState> >::iterator vector<std::map<unsigned long, NUISystem::CUIState> >::erase(vector<std::map<unsigned long, NUISystem::CUIState> >::iterator);
}