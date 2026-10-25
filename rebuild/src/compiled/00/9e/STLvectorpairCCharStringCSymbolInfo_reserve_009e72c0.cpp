// std::vector<pair<CCharString,CSymbolInfo>>::reserve(unsigned int n) -- STLport reserve for the symbol-map entry vector: when the capacity is below n, copy the entries into a fresh n-element block, destroy and free the old one (or just allocate when empty) and re-point the three words
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E72C0, length 186 bytes (`ret 4`).
// Label correction: the catalog's "Vector_Reserve" is STLport `vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::reserve`
//   (16-byte element, `sar 4`); it is called on the temporary of the landed CArray::DeallocateUnusedMemory 0x009E7C70.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the allocator), __uninitialized_copy @ 0x009E6270 (landed,
//   `__fastcall` first/last in ecx/edx, result + the __false_type tag reference on the stack), __destroy_aux @ 0x006FBCC0
//   (`__fastcall`, the range destroy of `_M_clear`).
// Shape: STLport 4.x reserve verbatim with `_M_allocate_and_copy`, `_M_clear` and `_M_set` inline (allocate = `n ? malloc(n *
//   16) : 0`, deallocate = null-tested free). Retail homes the copy's result AND its __false_type tag in the dead `n` slot
//   (n lives in ebp) while the old finish is spilled to the `push ecx` word: that tag-over-a-live-slot layout appears only
//   when __uninitialized_copy is a same-TU `auto_inline(off)` + `noinline` body (the landed 0x009E6270 loop) that never
//   touches its tag; declared-only, VC swaps the two frame words.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
};

class CSymbolMap {
public:
    class CSymbolInfo {                           // 12 bytes
    public:
        CCharString GroupName;                    // +0x00
        CCharString EntryName;                    // +0x04
        long        Val;                          // +0x08
    };
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};

struct __false_type { __false_type() {} };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x009E6270 (same-TU body)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x006FBCC0

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    pointer _M_allocate(size_type __n) { return __n != 0 ? (pointer)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_deallocate(pointer __p) { if (__p != 0) free(__p); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_deallocate(_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage = __e;
    }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = _M_allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void reserve(size_type __n);
};

template <class _Tp>
void vector<_Tp>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (_M_start) {
            __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _M_clear();
        } else {
            __tmp = _M_allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template class vector<pair<CCharString, CSymbolMap::CSymbolInfo> >;
}