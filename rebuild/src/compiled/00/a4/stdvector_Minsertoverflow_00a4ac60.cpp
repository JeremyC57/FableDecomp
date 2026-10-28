// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CLipSyncPhonemeSample>) STLport growth insert: allocate size + max(size, fill_len) samples, copy the head, x (once inline, or fill_len times) and the tail unless atend through the out-of-line uninitialized helpers, destroy and free the old block, adopt the new one
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@VCLipSyncPhonemeSample@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4AC60, 247 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): std::vector<CLipSyncPhonemeSample>::_M_insert_overflow -- its only caller is the landed
//   vector<CLipSyncPhonemeSample>::push_back 0x00A4AE00 (call at 0x00A4AE57), which binds this address as
//   ?_M_insert_overflow@?$vector@VCLipSyncPhonemeSample@@...@std@@QAEXPAVCLipSyncPhonemeSample@@ABV3@ABU__false_type@2@I_N@Z;
//   that spelling (public) is kept so the address has one name (STLport 4.6 declares it protected).
//   Element: Ego_r.pdb CLipSyncPhonemeSample { CCharString Symbol; unsigned long ID; CCountedPointer<CSpeechFrame> SampleFrame; }.
// Label: the catalog's "Vector_Reallocate_CCharString_CountedPointer" is descriptive.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x00A49BC0 (twice), CCharString copy ctor 0x0099EC30 (fill_len == 1
//   arm), __uninitialized_fill_n 0x00A49C10, __destroy_aux 0x00A4A2A0 (_M_clear's _Destroy), free 0x00BFEA14. The three
//   helpers are landed under stand-in names (UninitCopy_PairCharStringLongCountedPointer_00a49bc0,
//   UninitFillN_SPairCharStringLongCountedPointer_00a49c10, std::_Destroy_range<SNamedSurfaceEntry>); this source calls
//   them by their STLport names over the proven element (Review at landing).
// Shape: the landed G007 model 0x0084FAB0 (/Gr, same-TU noinline helper bodies so the tag is homed in the live atend
//   slot); this element's out-of-line string copy keeps __new_start in its argument slot ([esp+24h]).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }


class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
        Data = 0;
        Info = 0;
    }
};

class CCharString {
public:
    void* m_pData;                                     // +0x00
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

class CSpeechFrame;
class CLipSyncPhonemeSample {                          // Ego_r.pdb, 16 bytes
public:
    CCharString                   Symbol;              // +0x00
    unsigned long                 ID;                  // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;         // +0x08
};

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Out-of-line in retail (0x00A49BC0 / 0x00A49C10 / 0x00A4A2A0, their own COMDATs): same-TU noinline bodies.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                       const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>
__declspec(noinline) void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
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
public:                                                         // the bound (public) spelling of 0x00A4AC60
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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

typedef CLipSyncPhonemeSample _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);

} // namespace std