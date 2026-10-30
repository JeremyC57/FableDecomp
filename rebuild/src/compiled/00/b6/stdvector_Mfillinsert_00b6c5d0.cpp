// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<CVertexShader>) STLport fill-insert of n copies of a vertex-shader handle at position: shift in place when the capacity suffices (x copied first, its shared data add-ref'd and released at the end), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCVertexShader@@V?$allocator@VCVertexShader@@@std@@@std@@QAEXPAVCVertexShader@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6C5D0, 295 bytes (+ int3 padding), thiscall, `ret 0Ch`.
// Identity (proven): the landed resize 0x00B6CBD0 (std::vector<CVertexShader>::resize) binds exactly this spelling for its
//   call to 0x00B6C5D0; every callee is a landed vector<CVertexShader> helper whose notes name this caller:
//   __uninitialized_copy 0x00B6BAB0 (twice), __copy_backward 0x00B6B790, uninitialized_fill_n 0x00B6BB10 (the 3-argument
//   front end), fill 0x00B6B540 (twice), _M_insert_overflow 0x00B6C120. The element is CVertexShader (Ego_r.pdb, 8 bytes:
//   vptr 0x0129B3D0 (CShaderBase-derived), CVertexShaderData* Data; ReferenceCount at Data+0x10). The manifest label
//   "Vector_Insert_Generic" is descriptive.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim; here `__copy_backward_ptrs` is inline and `__copy_backward` is the
//   out-of-line call (the 0x00AE7340 split). Both empty tags are homed in the dead `__x` slot, which also carries the live
//   `__old_finish - __n` / `__n - __elems_after` values, so the tag-taking helpers are same-TU `__declspec(noinline)`
//   bodies that never read their tags (the live-slot tag rule). x_copy lives in the 8-byte frame; its inline copy
//   constructor stores the vtable and add-refs, its inline destructor releases with the virtual delete. CVertexShader's
//   operator= must be the inline FableWin.pdb Release/AddReference body (as in the landed fill 0x00B6B540): with the
//   same-TU helpers fully visible the destructor's own-vtable store becomes dead and Release() keeps Data in ecx for the
//   in-memory `dec [ecx+10h]` (a declared-only operator= leaves the store and splits the decrement).
// engine/CVertexShader.h is a flattened data-only record (no virtuals), so the polymorphic class is modelled locally as in
//   the landed 0x00B6CBD0 / 0x00B6C120 / 0x00B6B540 rows.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CVertexShaderData {                                   // Ego_r.pdb, 20 bytes (vtable 0x0129DA50)
    friend class CVertexShader;
public:
    virtual ~CVertexShaderData();                           // slot 0: deleting destructor 0x00A630D0
    void*       VertexDeclaration;                          // +0x04
    void*       VertexShader;                               // +0x08
    const void* FormatDecl;                                 // +0x0C
    unsigned long ReferenceCount;                           // +0x10
private:
    void AddReference() { ++ReferenceCount; }
    void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
};

class CShaderBase {                    // vtable 0x0129A030 (abstract: never stored by a derived ctor/dtor)
public:
    CShaderBase() { }
    CShaderBase(const CShaderBase&) { }
    virtual ~CShaderBase() { }
};

class CVertexShader : public CShaderBase {                  // vtable 0x0129B3D0, 8 bytes
public:
    CVertexShaderData* Data;                                // +0x04
    CVertexShader(const CVertexShader& other) : CShaderBase(other), Data(other.Data)
    {
        if (Data)
            Data->AddReference();
    }
    const CVertexShader& operator=(const CVertexShader& rhs)
    {
        if (Data)
            Data->Release();
        Data = rhs.Data;
        Data->AddReference();
        return *this;
    }
    virtual ~CVertexShader()
    {
        if (Data)
            Data->Release();
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
{                                                                                              // 0x00B6BAB0
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
{                                                                                              // 0x00B6BB10
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00B6B790
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x00B6B540
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
                            bool __atend = false);                                             // 0x00B6C120
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

template void vector<CVertexShader>::_M_fill_insert(CVertexShader*, unsigned int, const CVertexShader&);

} // namespace std