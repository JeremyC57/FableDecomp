// std::vector::_M_insert_overflow(CAbilityTrackDef* __position, const CAbilityTrackDef& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<CAbilityTrackDef>) growth insert
// Leaf: ?_M_insert_overflow@?$vector@VCAbilityTrackDef@@V?$allocator@VCAbilityTrackDef@@@std@@@std@@IAEXPAVCAbilityTrackDef@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047A5FF, 232 bytes, thiscall, `ret 14h` (frame-pointer size module).
// Identity (proven): its only caller is the landed vector<CAbilityTrackDef>::_M_fill_insert 0x0047A4A2; the copy constructor 0x004786CF is bound ??0CAbilityTrackDef@@QAE@ABV0@@Z.
// Callees: malloc 0x00BFEA0E, CAbilityTrackDef copy constructor 0x004786CF, vector<CAbilityTrackDef>::_M_clear 0x004786AC.
// Shape (family M064, representative 0x004763AF): the M052 model (landed 0x008D61A0) with the record's out-of-line copy
//   constructor and _M_clear left out of line; `optimize("y", off)` + `optimize("s", on)`.
// Header debt: the record is modelled by its retail size (0x50) with its copy constructor and a virtual destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CAbilityTrackDef {
public:
    CAbilityTrackDef(const CAbilityTrackDef& other);                 // 0x004786CF
    virtual ~CAbilityTrackDef();
    unsigned char _body[0x50 - 4];
};
namespace std {

struct __true_type { };
struct __false_type { };

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    __forceinline size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                      // out of line
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

template void vector<CAbilityTrackDef >::_M_insert_overflow(CAbilityTrackDef*, const CAbilityTrackDef&, const __false_type&, unsigned int, bool);

} // namespace std