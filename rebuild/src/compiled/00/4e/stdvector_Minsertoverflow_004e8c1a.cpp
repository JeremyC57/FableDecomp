// std::vector::_M_insert_overflow(pointer __position, const value_type& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<CDefIndex >) STLport growth insert for a 4-byte element: allocate size + max(size, fill_len), copy the head, x (once or fill_len times) and the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@IAEXPAVCDefIndex@@AB
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E8C1A, 207 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 14h`.
// Identity: std::vector<CDefIndex >::_M_insert_overflow -- proven; evidence: its only caller is this chunk's (G064)
//   std::vector<CDefIndex >::_M_fill_insert 0x004E8AF4 (overflow branch), reached from CContainerRewardHeroDef::Transfer 0x004E8987 (ObjectFamilies +0x28). STLport spelling.
//   Family G065 (the seven overflow inserts of G064; catalogue "VectorReallocateAndMove" is behavioural).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 (everything else inline).
// Shape: STLport 4.6 vector.c verbatim (no EH), the G009 body (0x006E4FC0) in a frame-pointer /Os TU: old size in the
//   one local, new_start homed in the dead fill_len slot, the copies strength-reduced; /Os needs __forceinline helpers.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class CDefIndex { public: long Index; };                 // 4-byte def index, trivial copy
namespace std {
struct __false_type { };
template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    __forceinline void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
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
typedef CDefIndex _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
}