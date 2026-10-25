// std::vector::_M_insert_overflow(CAnimInit* position, const CAnimInit& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport regrowth of a vector of 0x2C-byte animation inits: allocate max(2*size, size+fill_len), copy the head, fill_len copies of x and (unless at the end) the tail, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5100, 310 bytes (+ int3 padding to 320), thiscall, `ret 14h`.
// Identity: the catalog's "Vector_Resize_0x2c_elements" is a descriptive auto label. The element is CAnimInit (Ego_r.pdb,
//   0x2C bytes: UpdateMode, Type, Looping, Anim, Speed, Alpha, LastAlpha, Frame, Flags, TypeFlags, Priority): the only
//   caller is the vector's push_back 0x006E5390 (landed), used by the phoneme-animation update 0x006E3B90 to collect the
//   CAnimInit records it builds (ctor 0x0070C050).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14; everything else is inline.
// Shape: STLport 4.6 vector.c `_M_insert_overflow(.., const __false_type&, ..)` verbatim. CAnimInit is a plain record but
//   STLport's __type_traits default to __false_type, so the copies are placement-new loops (null test + `rep movsd` of
//   11 dwords, edi/esi reloaded from the frame after each block move); both __uninitialized_copy calls and
//   __uninitialized_fill_n are `__forceinline` (inlined like retail), the _M_clear destroy loop is empty and vanishes,
//   leaving the null-tested free. The new block is homed in the `push ecx` word frame and the old `this` spilled.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

enum EAnimUpdateMode { ANIM_UPDATE_MODE_0 };

class CAnimFrameInfo {
public:
    float Current;                                          // +0x00
    float Last;                                             // +0x04
};

class CAnimInit {                                           // 0x2C bytes, implicit copy (STLport: __false_type)
public:
    EAnimUpdateMode UpdateMode;                             // +0x00
    unsigned char   Type;                                   // +0x04
    bool            Looping;                                // +0x05
    long            Anim;                                   // +0x08
    float           Speed;                                  // +0x0C
    float           Alpha;                                  // +0x10
    float           LastAlpha;                              // +0x14
    CAnimFrameInfo  Frame;                                  // +0x18
    unsigned long   Flags;                                  // +0x20
    unsigned long   TypeFlags;                              // +0x24
    long            Priority;                               // +0x28
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
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

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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

template void vector<CAnimInit>::_M_insert_overflow(CAnimInit*, const CAnimInit&, const __false_type&,
                                                      unsigned int, bool);

} // namespace std