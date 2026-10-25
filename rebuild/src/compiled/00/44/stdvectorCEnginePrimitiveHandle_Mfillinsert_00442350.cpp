// std::vector<CEnginePrimitiveHandle>::_M_fill_insert(CEnginePrimitiveHandle* position, unsigned int n, const CEnginePrimitiveHandle& x) -- STLport insert of n copies of a primitive handle: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCEnginePrimitiveHandle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00442350, 351 bytes, thiscall `ret 0Ch`; /O2.
// Identity: stride 8 with vptr 0x0122F598 and the landed out-of-line helpers of this element (0x004400C0, 0x00440390,
//   0x00440100, 0x00440180); the growth path 0x004413C0 is this chunk's _M_insert_overflow. Callers: insert(position, n,
//   x) (tail jump 0x00442950), 0x004434E8, 0x00443FE7 (resize). The catalog label "Vector_InsertRangeWithRealloc" is
//   wrong: this is _M_fill_insert.
// Callees: __uninitialized_copy 0x004400C0 (twice), __copy_backward(first, last, result, const random_access_iterator_tag&,
//   int*) 0x00440390, fill(first, last, x) 0x00440100 (both fills tail-merged), uninitialized_fill_n(first, n, x)
//   0x00440180, the primitive's virtual slots 3 (OnLastHandleReleased) and 1 (Destroy) and ~CBaseClass 0x0099A2E0 in the
//   inlined x_copy destructor, _M_insert_overflow 0x004413C0; all free helpers __fastcall.
// Shape: STLport 4.x `_M_fill_insert` verbatim, as 0x00411DB0: x_copy is an 8-byte frame object built by the inline
//   copy constructor (vptr, add-ref) and destroyed inline at the end of the in-place branch (a `__forceinline` destructor -- VC7.1 calls a
//   virtual destructor of a local out of line otherwise; vptr store, IsUnique ->
//   slot 3, the null-tested release, the zero store, the out-of-line base destructor, as the landed 0x00BBDCB0). Every
//   empty tag sits on the `x` argument slot ([esp+24h]), which needs the same-TU helper bodies (GOTCHAS 504/667).
// Model: Ego_r.pdb CEnginePrimitiveHandle (vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04) with the
//   counted-pointer accessors of the landed 0x00BBDCB0; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                              // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0C)
    long RefCount;                                                       // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {                                 // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;               // +0x00
    CIVCountedPointer(const CIVCountedPointer& x) : Object(x.Object) { if (Object) ++Object->RefCount; }
    CIVCountedPointer& operator=(const CIVCountedPointer& x);
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

class CBaseClass {                                        // vtable 0x01231710
public:
    virtual ~CBaseClass();                                // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass {        // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                               // +0x04
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), Data(x.Data) { }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
    }
    virtual void v001();
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x004400C0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)   // 0x00440180
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x00440390
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)        // 0x00440100
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x004413C0
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
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

template void vector<CEnginePrimitiveHandle>::_M_fill_insert(CEnginePrimitiveHandle*, unsigned int, const CEnginePrimitiveHandle&);

}