// std::vector::_M_insert_overflow(CIDrawEnvironmentParams::CThemeChannel* position, const CThemeChannel& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport regrowth of the environment theme-channel vector (0x1F0-byte elements): allocate max(2*size, size+fill_len), copy the head, fill_len copies of x and (unless at the end) the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6990, 257 bytes, thiscall, `ret 14h`, no EH frame.
// Identity: the catalog's "CFeatDefVector_ReserveAndCopy" is a descriptive auto label (CFeatDef is 100 bytes). The
//   element is CIDrawEnvironmentParams::CThemeChannel (Ego_r.pdb: Colours CRGBFloatColour[19] +0, Scalers float[30]
//   +0x130, TextureSets CIDrawEnvironmentTextureSet[3] +0x1A8; 0x1F0 bytes with retail's 12-byte STLport vectors),
//   identified by the landed __destroy_aux 0x006B6610 and __uninitialized_copy 0x006B6500 this body calls. The only
//   caller is the vector's _M_fill_insert 0x006B6B30 (call site 0x006B6C64).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x006B6500 (twice),
//   CThemeChannel's implicit copy constructor 0x006B6010 (the fill_len == 1 _Construct and the inlined fill_n loop),
//   __destroy_aux(first, last, const __false_type&) 0x006B6610 (_M_clear's _Destroy), free 0x00BFEA14.
// Shape: STLport 4.6 vector.c `_M_insert_overflow(.., const __false_type&, ..)` verbatim: const-reference `(max)`
//   (`lea ecx,[esp+24h] / jb / lea ecx,[esp+10h]`), the new block homed in the dead fill_len slot, the fill_len == 1
//   placement copy, otherwise __uninitialized_fill_n inlined (`test ebx,ebx / jbe` + a placement-new loop), the tail copy
//   unless atend, then `_M_clear()` and `_M_set`. The two tag-taking callees are same-TU noinline bodies that never touch
//   the tag, so every `__false_type()` temporary is homed in the live atend argument slot (`lea ...,[esp+28h]`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

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

    protected:
    void _M_clear();
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CRGBFloatColour {
public:
    float R, G, B, A;
};

struct CIDrawEnvironmentTextureSet {
public:
    CArray<long>  Textures;                                 // +0x00
    CArray<float> Blends;                                   // +0x0C
};

struct CIDrawEnvironmentParams {
public:
    struct CThemeChannel {                                   // 0x1F0 bytes, implicit copy constructor @ 0x006B6010
    public:
        CRGBFloatColour             Colours[19];            // +0x000
        float                       Scalers[30];            // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];         // +0x1A8
    };
};

namespace std {

// Same-TU stand-ins for 0x006B6500 / 0x006B6610 (their own landed rows), kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

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

typedef CIDrawEnvironmentParams::CThemeChannel _ThemeChannel;
template void vector<_ThemeChannel>::_M_insert_overflow(_ThemeChannel*, const _ThemeChannel&, const __false_type&,
                                                        unsigned int, bool);

} // namespace std