// std::vector::_M_insert_overflow(pair<long,CSoundBankRecord>* position, const pair<long,CSoundBankRecord>& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport growth path of the sound manager's bank vector: allocate old_size + max(old_size, fill_len) entries, copy the head, construct the fill_len copies of x, copy the tail unless appending, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00993310, 272 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: the catalog's "Vector_InsertRange_Filter" is descriptive. The element is std::pair<long, CSoundBankRecord>
//   (0x24 bytes; the landed sibling helpers 0x00992750 / 0x009927F0 / 0x00992510 name it): the entries of the
//   CASoundManager bank vector map. Callers: the push_back / insert paths 0x00991BC3, 0x00991D25, 0x0099371C, 0x0099383B.
// Callees: __uninitialized_copy(first, last, result, const __false_type&) 0x00992750 (twice), __uninitialized_fill_n(
//   first, n, x, const __false_type&) 0x009927F0 (both `__fastcall`, `ret 8`, landed), the record's copy constructor
//   0x009922F0 (landed; called out of line for the single-element arm), ~CSoundBankRecord 0x00992510 (landed),
//   malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape: STLport 4.6 `_M_insert_overflow` (non-POD arm, no exceptions): `(max)` returns a reference (the `lea ecx`
//   pair choosing the old-size local or the fill_len slot), allocate() tests `__len != 0`, the new start is homed in the
//   dead fill_len slot, the single-element arm is the placement-new `_Construct` (null test), and _M_clear's
//   `_Destroy` loop + deallocate (null-tested free) precede _M_set.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};

class CCPPointerInfo {
public:
    long RefCount;                              // +0x00
};

template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
};

class CSoundBank;
class CSoundBankData;

// The 0x20-byte sound-bank record (descriptive, as the landed ~CSoundBankRecord @ 0x00992510).
class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
    CSoundBankRecord(const CSoundBankRecord& other);                // 0x009922F0
    ~CSoundBankRecord();                                            // 0x00992510
};

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;                                   // +0x00
    T2 second;                                  // +0x04
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

// Same-TU stand-ins for the landed helpers, kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                               // 0x00992750
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                               // 0x009927F0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<pair<long, CSoundBankRecord> >::_M_insert_overflow(pair<long, CSoundBankRecord>*,
    const pair<long, CSoundBankRecord>&, const __false_type&, unsigned int, bool);

}   // namespace std