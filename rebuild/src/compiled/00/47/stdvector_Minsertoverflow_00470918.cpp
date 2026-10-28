// std::vector::_M_insert_overflow(pointer __position, const value_type& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<pair<unsigned long, CSoundPair> >) STLport growth path: allocate old+max(old,n), copy the head, construct n copies of x, copy the tail unless inserting at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@KVCSoundPair@@@std@@V?$allocator@U?$pair@KVCSoundPair@@@std@@@2@@std@@IAEXPAU?$pair@KVCSoundPair@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00470918, 272 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity (probable): its only caller is the _M_fill_insert 0x00470778 of the same vector (row of this chunk), whose
//   resize 0x004706F6 serves CSoundMap::SoundMap (Ego_r.pdb CVectorMap<unsigned long, CSoundPair>).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape: the landed K100 _M_insert_overflow model (0x0046B30C / 0x0046AECC) with a 12-byte pair whose explicit copy
//   constructor gives three dword moves per null-checked placement copy; max() by reference picks the slot
//   (`lea eax,[n] / lea eax,[old]`).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

inline void* operator new(unsigned int, void* where) { return where; }

class CSoundPair {                                       // Ego_r.pdb 8 bytes
public:
    long FirstSound;
    long LastSound;
};

namespace std {
struct __false_type { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport 4.x's explicit copy constructor
};
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class T> __forceinline T* __uninitialized_copy(T* first, T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class T> __forceinline T* __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    __forceinline void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
protected:
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
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef pair<unsigned long, CSoundPair> _Elem;
template class vector<_Elem>;
}