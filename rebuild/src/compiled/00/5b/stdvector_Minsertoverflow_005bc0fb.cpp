// std::vector::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<std::vector<std::pair<long, long> > >) STLport growth insert: allocate size + max(size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless atend, release the old block, adopt the new one
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@V?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@V?$allocator@V?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@2@@std@@IAEXPAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@2@ABV32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC0FB, 176 bytes (size-optimised TU with a frame pointer, odd entry), thiscall, `ret 14h`.
// Identity (probable): std::vector<std::vector<std::pair<long, long> > >::_M_insert_overflow (the non-POD overload); evidence: its fill_len == 1 constructor 0x005BAEC4 is a vector copy constructor (_Vector_base(n) 0x005128B0 allocating n * 8 bytes, then __uninitialized_copy 0x005BAA1C moving two dwords per element); its only caller is the push_back 0x005BC237, called six times each by CTCInventoryClothing::ConstructScreenPC 0x005B8743 and its console twin 0x005B76D1, which build one list per clothing category (Ego_r.pdb CTCInventoryClothing::ConstructClothingList takes vector<pair<long, long> >&, and Ego_r.pdb instantiates vector<vector<pair<long, long> > >::push_back)
// Label: the manifest's "Vector_ReserveAndShift_BodyPartMesh" is a propagated / behavioural label (family J071: five 176-byte growth inserts, each its own element).
// Callees: allocator::allocate(n, const void* = 0) 0x005B9EF3 (out of line on _M_end_of_storage), __uninitialized_copy(first, last,
//   result, const __false_type&) 0x005BB1CF (__fastcall, head and tail), the element copy constructor 0x005BAEC4 (behind the placement
//   null test, fill_len == 1), __uninitialized_fill_n(first, n, x, const __false_type&) 0x005BB1FA (__fastcall), _M_clear() 0x005BC0A3.
// Shape: STLport 4.6 _vector.c `_M_insert_overflow` (non-POD, no EH) verbatim: `(max)` through its references, old size and then
//   new_finish in the frame. The constructor-less __false_type tags get no store and share the padding of the `atend` slot
//   ([ebp+1Bh]) because this TU compiles both callee bodies (STLport's loops, instantiated above) ahead of the growth insert.
// Element: std::vector<std::pair<long, long> > (12 bytes) -- probable: the inner 8-byte element is copied as two dwords and is named from the Ego_r.pdb vector<vector<pair<long, long> > > instantiations and CTCInventoryClothing::ConstructClothingList(vector<pair<long, long> >&, ...), not from RTTI
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0);    // 0x005B9EF3
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
template <class _InputIter, class _ForwardIter>                    // 0x005BB1CF for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>               // 0x005BB1FA for this element (compiled ahead of the growth insert)
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
    vector(const vector<_Tp, _Alloc>& __x);              // 0x005BAEC4 for vector<pair<long, long> >
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                     // 0x005BC0A3
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
namespace std {
template vector<pair<long, long> >* __uninitialized_copy<vector<pair<long, long> >*, vector<pair<long, long> >*>(vector<pair<long, long> >*, vector<pair<long, long> >*, vector<pair<long, long> >*, const __false_type&);
template vector<pair<long, long> >* __uninitialized_fill_n<vector<pair<long, long> >*, unsigned int, vector<pair<long, long> > >(vector<pair<long, long> >*, unsigned int, const vector<pair<long, long> >&, const __false_type&);
template void vector<vector<pair<long, long> > >::_M_insert_overflow(vector<pair<long, long> >*, const vector<pair<long, long> >&, const __false_type&, unsigned int, bool);
}