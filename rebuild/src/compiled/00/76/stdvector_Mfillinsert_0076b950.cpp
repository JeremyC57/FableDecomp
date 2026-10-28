// std::vector::_M_fill_insert(CDefPointer<const CThingBaseDef>* pos, size_type n, const CDefPointer<const CThingBaseDef>& x) -- STLport fill-insert of n add-ref'd copies of a thing-def pointer at pos: shift in place when the capacity suffices, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B950, length 288 bytes (thiscall, `ret 0Ch`).
// Identity: the catalog label "Vector_Resize_IVCountedPointer" is descriptive. The only caller is vector::resize
//   0x0076BF80 (CTCObjectAugmentations' TU); the growth callee is the landed vector<CDefPointer<const CThingBaseDef> >
//   ::_M_insert_overflow 0x005B3275 (CTCObjectAugmentations::Augmentations, Ego_r.pdb), whose __uninitialized_copy
//   0x005B1479 this body also calls (STLport COMDATs the linker took from that TU).
// Callees: __uninitialized_copy 0x005B1479 (twice), __copy_backward 0x00769210 (random_access_iterator_tag, null
//   distance), fill 0x007691A0, uninitialized_fill_n 0x007688B0 (the three-argument front end), _M_insert_overflow
//   0x005B3275; the pointee's slot-1 Destroy (x_copy's inline release).
// Shape: STLport 4.x vector.c `_M_fill_insert` verbatim (as 0x00770990): x_copy (the inline add-ref copy) lives in the
//   dead `x` slot and its inline release ends the in-place arm; `__copy_backward_ptrs` is inline here and forwards to
//   the out-of-line `__copy_backward(.., random_access_iterator_tag(), (int*)0)`, while `uninitialized_fill_n` stays an
//   out-of-line call. The tag-taking helpers are same-TU `__declspec(noinline)` bodies that never read their tags.
inline void* operator new(unsigned int, void* p) { return p; }

class CDefPointeeBase {                                     // counted def object: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_refCount;                                        // +0x04
};

class CThingBaseDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb)
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->m_refCount == 0)
                Object->Destroy();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        CDefPointeeBase* old = Object;
        if (old)
            if (--old->m_refCount == 0)
                old->Destroy();
        Object = other.Object;
        if (Object)
            ++Object->m_refCount;
        return *this;
    }
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x005B1479
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                              // 0x007688B0
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00769210
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x007691A0
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x005B3275
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CDefPointer<const CThingBaseDef> >::_M_fill_insert(CDefPointer<const CThingBaseDef>*, unsigned int,
                                                                        const CDefPointer<const CThingBaseDef>&);

}