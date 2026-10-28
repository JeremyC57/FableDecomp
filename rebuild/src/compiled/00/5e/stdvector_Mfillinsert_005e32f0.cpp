// std::vector::_M_fill_insert(pair<unsigned long, CAnimationEntry>* position, unsigned int n, const pair& x) -- STLport insert of n copies of a CRC-keyed animation entry: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E32F0, 339 bytes (337 without padding), thiscall `ret 0Ch`. Callers: the
//   insert(pos, n, x) jmp thunk 0x005E3710 and vector<pair<unsigned long, CAnimationEntry> >::resize(n, x) 0x005E39E0
//   (`call` at 0x005E3A3F; landed resize(n) 0x005E3C60 forwards to it) -- CAnimationSet::Anims' element.
// Callees (`__fastcall`, COMDAT-folded across 20-byte element types, hence their propagated labels):
//   __uninitialized_copy(.., const __false_type&) 0x005E16F0, __copy_backward(.., const random_access_iterator_tag&,
//   int*) 0x005E0E70, __uninitialized_fill_n(.., const __false_type&) 0x005E1740 (landed), fill 0x005E1820 (landed),
//   _M_insert_overflow(.., const __false_type&, n, atend) 0x005E31F0 (landed; atend defaulted false: `push 0`); x_copy's
//   destructor is the inlined counted release (`dec [ecx+4] ; jne ; call [vt+4]`).
// Label correction: the catalog "std_Vector_Resize" -- this is STLport 4.6 `_M_fill_insert` (resize is 0x005E39E0).
// Model: as the landed 0x005E36B0 / 0x005E1740: pair<unsigned long, CAnimationEntry> (20 bytes) with STLport's
//   memberwise pair copy constructor; CAnimationEntry = BankIndex, two one-word CDefStrings and a
//   CIVCountedPointer<CAnimComponentMap> (intrusive count at +4, virtual delete at slot 1).
// Shape: STLport 4.6 `_M_fill_insert` verbatim; x_copy is a 20-byte frame copy (add-ref inline); same-TU noinline helper
//   bodies (auto_inline off) let VC7.1 home the empty tags and `old_finish - n` / `n - elems_after` in the dead n slot;
//   the two branches' first tag push is hoisted above the branch, the two `fill` calls tail-merge.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

namespace std {
template <class _T1, class _T2> struct pair {              // STLport 4.6 pair: explicit memberwise copy constructor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x005E16F0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x005E1740
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x005E0E70
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x005E1820
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x005E31F0
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

template void vector<pair<unsigned long, CAnimationEntry> >::_M_fill_insert(pair<unsigned long, CAnimationEntry>*, unsigned int, const pair<unsigned long, CAnimationEntry>&);
}