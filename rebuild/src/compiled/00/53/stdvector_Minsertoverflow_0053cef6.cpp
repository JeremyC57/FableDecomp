// std::vector::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<std::map<unsigned long, NUISystem::CUIState> >) STLport growth insert: allocate size + max(size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless atend, release the old block, adopt the new one
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@V?$allocator@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@@2@@std@@IAEXPAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@2@ABV32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CEF6, 176 bytes (size-optimised TU with a frame pointer), thiscall, `ret 14h`.
// Identity (proven): std::vector<std::map<unsigned long, NUISystem::CUIState> >::_M_insert_overflow (the non-POD overload); evidence: its fill_len == 1 constructor 0x0053CB9B is the _Rb_tree copy constructor of the map<unsigned long, NUISystem::CUIState> tree (0x38-byte header node, _M_copy 0x0042CB90 -- the tree whose operator= 0x0042CE7A the J070 row 0x0053CC38 calls); its callers are the push_back 0x0053CFC2 of NUISystem::CList::ScrollingChildrenStates (+0x17C, Ego_r.pdb vector<map<unsigned long, CUIState> >) and CFrontEndList::Initialise 0x0054D660
// Label: the manifest's "CActiveFile::Read_CArrayElements" is a propagated / behavioural label (family J071: five 176-byte growth inserts, each its own element).
// Callees: allocator::allocate(n, const void* = 0) 0x00535E5A (out of line on _M_end_of_storage), __uninitialized_copy(first, last,
//   result, const __false_type&) 0x0053CCA5 (__fastcall, head and tail), the element copy constructor 0x0053CB9B (behind the placement
//   null test, fill_len == 1), __uninitialized_fill_n(first, n, x, const __false_type&) 0x0053CCD0 (__fastcall), _M_clear() 0x0053CDA0.
// Shape: STLport 4.6 _vector.c `_M_insert_overflow` (non-POD, no EH) verbatim: `(max)` through its references, old size and then
//   new_finish in the frame. The constructor-less __false_type tags get no store and share the padding of the `atend` slot
//   ([ebp+1Bh]) because this TU compiles both callee bodies (STLport's loops, instantiated above) ahead of the growth insert.
// Element: std::map<unsigned long, NUISystem::CUIState> (STLport, 12 bytes); its implicit copy constructor is the _Rb_tree copy constructor
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0);    // 0x00535E5A
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>                    // 0x0053CCA5 for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>               // 0x0053CCD0 for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                     // 0x0053CDA0
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}   // namespace std
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
namespace NUISystem {
class CUIState {                                         // Ego_r.pdb 0x24 bytes
public:
    unsigned long StateChangeFlag;                       // +0x00
    long          StateChangeType;                       // +0x04 EStateChangeType
    C2DVector     Position;                              // +0x08
    C2DVector     Zoom;                                  // +0x10
    unsigned long Colour;                                // +0x18 CRGBColour
    float         UpdateTime;                            // +0x1C
    bool          LinearChange;                          // +0x20
};
}
namespace std {
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {                                         // 12 bytes: header node pointer, node count, comparator
public:
    void*        _M_header;                              // +0x0
    unsigned int _M_node_count;                          // +0x4
    _Compare     _M_key_compare;                         // +0x8
    _Rb_tree(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);   // 0x0053CB9B
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                              // STLport, 12 bytes; its implicit copy constructor copies _M_t
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
namespace std {
template map<unsigned long, NUISystem::CUIState>* __uninitialized_copy<map<unsigned long, NUISystem::CUIState>*, map<unsigned long, NUISystem::CUIState>*>(map<unsigned long, NUISystem::CUIState>*, map<unsigned long, NUISystem::CUIState>*, map<unsigned long, NUISystem::CUIState>*, const __false_type&);
template map<unsigned long, NUISystem::CUIState>* __uninitialized_fill_n<map<unsigned long, NUISystem::CUIState>*, unsigned int, map<unsigned long, NUISystem::CUIState> >(map<unsigned long, NUISystem::CUIState>*, unsigned int, const map<unsigned long, NUISystem::CUIState>&, const __false_type&);
template void vector<map<unsigned long, NUISystem::CUIState> >::_M_insert_overflow(map<unsigned long, NUISystem::CUIState>*, const map<unsigned long, NUISystem::CUIState>&, const __false_type&, unsigned int, bool);
}