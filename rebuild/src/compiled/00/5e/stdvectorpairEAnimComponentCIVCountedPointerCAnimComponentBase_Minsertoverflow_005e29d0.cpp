// std::vector<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- STLport regrowth: allocate max(2*size, size+fill_len) 8-byte pairs, copy the head, fill_len copies of x and (unless at the end) the tail, release the old block
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@W4EAnimComponent@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E29D0, 219 bytes, thiscall, `ret 14h`, no EH frame.
// Identity: the manifest's "std::vector::_Reallocate" is descriptive. The element is the 8-byte
//   pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > of the anim-component CVectorMap: the callees are that
//   pair's landed __uninitialized_copy 0x005E0C60 (callers 0x005E2A1D / 0x005E2A73 are this body),
//   __uninitialized_fill_n 0x005E0CA0 (caller 0x005E2A57) and __destroy_aux 0x005E0F00; the inline copy adds a
//   reference at pointee +4 (CIVCountedPointeeBase). The 21 callers in 0x005DD28D..0x005E3671 are the push_back /
//   insert paths of the CAnimationSet TU.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x005E0C60,
//   __uninitialized_fill_n(first, n, x, const __false_type&) 0x005E0CA0, __destroy_aux(first, last,
//   const __false_type&) 0x005E0F00 (all `__fastcall`, the tag by reference), free 0x00BFEA14.
// Shape: STLport 4.6 vector.c `_M_insert_overflow(.., const __false_type&, ..)` verbatim (no _STLP_TRY: no EH):
//   `len = old_size + (max)(old_size, fill_len)` through references (old_size homed in the `push ecx` word), the
//   allocate null test, the fill_len == 1 copy inline (placement-new null test + the counted add-ref), then
//   `_M_clear()` (`_Destroy` + the null-tested free) and `_M_set`. Every tag-taking callee is handed the address of the
//   LIVE `atend` argument slot, which needs same-TU noinline stand-in bodies that never touch the tag (with
//   declaration-only callees VC7.1 homes the tag in a frame word instead).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase {                                  // CIVCountedPointeeBase-derived: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Delete();                                  // slot 1: destroy on the last reference
    long IVRefCount;                                        // +0x04
};

template <class T> class CIVCountedPointer {                // one word: the intrusively counted object
public:
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU stand-ins for 0x005E0C60 / 0x005E0CA0 / 0x005E0F00 (their own landed rows), kept out of line.
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
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

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

    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
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

typedef pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > _AnimComponentPair;
template void vector<_AnimComponentPair>::_M_insert_overflow(_AnimComponentPair*, const _AnimComponentPair&,
                                                             const __false_type&, unsigned int, bool);

} // namespace std