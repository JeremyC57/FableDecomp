// std::vector::_M_fill_insert(CIDrawEnvironmentParams::CThemeChannel* position, unsigned int n, const CThemeChannel& x) -- STLport insert of n copies of an environment theme channel: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6B30, 325 bytes, thiscall, `ret 0Ch`, 0x1F8-byte frame (x_copy).
// Identity: the catalog's "CFeatDefVector_Resize" is a descriptive auto label (CFeatDef is 100 bytes). The element is
//   CIDrawEnvironmentParams::CThemeChannel (0x1F0 bytes in retail, see 0x006B6010); every callee is a landed row of the
//   same vector: copy ctor 0x006B6010, __uninitialized_copy 0x006B6500, __copy_backward 0x006B6250, fill 0x006B6560,
//   uninitialized_fill_n 0x006B6590, ~CThemeChannel 0x006B5B80 and _M_insert_overflow 0x006B6990.
// Callees: as above (all helpers `__fastcall` from the /Gr TU, the tags by reference; _M_insert_overflow thiscall with
//   atend defaulted to false: `push 0`).
// Shape: STLport 4.6 `_M_fill_insert` verbatim: `_Tp __x_copy = __x;` (the out-of-line implicit copy constructor into the
//   frame), `elems_after = finish - position`, the `elems_after > n` slide (copy the last n into the spare room, move the
//   rest back with __copy_backward_ptrs -> __copy_backward(.., random_access_iterator_tag(), (int*)0), fill the gap) or
//   the short-tail path (uninitialized_fill_n of the overhang, copy the tail after it, fill the old tail); both fill calls
//   tail-merge before x_copy's out-of-line destructor. The helpers are same-TU noinline bodies (auto_inline off), so VC7.1
//   keeps the empty tags and the spilled `old_finish - n` / `n - elems_after` words in the frame as retail does.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };


inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }

    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               __true_type());
    }
    ~vector() { }

    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x006B6990
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CRGBFloatColour {
public:
    float R, G, B, A;
};

class CIDrawEnvironmentTextureSet {
public:
    CArray<long>  Textures;                                 // +0x00
    CArray<float> Blends;                                   // +0x0C
};

class CIDrawEnvironmentParams {
public:
    class CThemeChannel {                                   // 0x1F0 bytes, implicit copy constructor @ 0x006B6010
    public:
        CRGBFloatColour             Colours[19];            // +0x000
        float                       Scalers[30];            // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];         // +0x1A8
    };
};


namespace std {

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-ins for the landed helpers, kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                  // 0x006B6500
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                  // 0x006B6250
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                  // 0x006B6560
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                  // 0x006B6590
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

typedef CIDrawEnvironmentParams::CThemeChannel _ThemeChannel;
template void vector<_ThemeChannel>::_M_fill_insert(_ThemeChannel*, unsigned int, const _ThemeChannel&);

} // namespace std