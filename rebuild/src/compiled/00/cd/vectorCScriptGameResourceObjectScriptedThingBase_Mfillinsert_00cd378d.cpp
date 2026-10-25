// vector<CScriptGameResourceObjectScriptedThingBase>::_M_fill_insert(iterator pos, size_type n, const T& x) -- STLport fill insert: n copies of x at pos, in place when the spare capacity suffices, else through the overflow reallocation
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD378D, 218 bytes, `__fastcall` (vector in ecx; pos, n and the value
//   reference on the stack: `ret 0Ch`).
// Label: the catalog name "CActiveFile_LoadAndInsertMeshArray" is descriptive; the body is STLport 4.6's
//   vector::_M_fill_insert (_vector.c) for the 16-byte scripted-thing resource object (copy constructor 0x00905330,
//   destructor 0x007E74D0, vtable 0x0127094C).
// Callees: CScriptGameResourceObjectScriptedThingBase copy constructor @ 0x00905330 / destructor @ 0x007E74D0 (the
//   x_copy local); __uninitialized_copy(first, last, result, __false_type) @ 0x00CD25E8 (x2), __copy_backward(first,
//   last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x00CD2C7C, __uninitialized_fill_n(first, n, x,
//   __false_type) @ 0x00CD2613, fill(first, last, x) @ 0x00CD2AFD, _M_insert_overflow(pos, x, __false_type, n, false)
//   @ 0x00CD36DF -- all `__fastcall` helpers of the /Gr build, out of line in the size TU (the four tag/value
//   helpers carry same-TU `noinline` bodies; only the overflow insert stays a declaration).
// Shape: frame-pointer size TU. Every empty tag temporary shares the dead argument slot [ebp+0Bh] (the shared
//   `lea eax,[ebp+0Bh] ; push eax` is hoisted above the elems_after branch); old_finish is homed in the dead value
//   slot [ebp+10h] once x has been copied; __copy_backward_ptrs is inline over the out-of-line __copy_backward. The
//   tags sit on the LIVE pos slot only because VC7.1 sees the helpers' bodies never read them: with declaration-only
//   helpers the lane moves old_finish to the n slot [ebp+0Ch] and the tags to [ebp+13h] (218 v 218, slots only).
class CScriptGameResourceObjectScriptedThingBase {      // 0x10 bytes, vtable 0x0127094C
public:
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x00905330
    virtual ~CScriptGameResourceObjectScriptedThingBase();                                              // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);
    void* m_pIntelligentPointer;
    void* m_pImplementation;
    void* m_pImplementationInfo;
};
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T> class allocator { };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)   // 0x00CD25E8
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)   // 0x00CD2613
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter first, _RandomAccessIter last, _BidirectionalIter result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x00CD2C7C
{
    for (_Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter first, _ForwardIter last, const _Tp& val)   // 0x00CD2AFD
{
    for (; first != last; ++first)
        *first = val;
}
#pragma auto_inline(on)
template <class _Tp>
inline _Tp* __fastcall __copy_backward_ptrs(_Tp* first, _Tp* last, _Tp* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    typedef unsigned int size_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend = false);   // 0x00CD36DF
    void _M_fill_insert(iterator position, size_type n, const T& x);
};
template <class T, class A>
void vector<T, A>::_M_fill_insert(iterator position, size_type n, const T& x)
{
    if (n != 0) {
        if (size_type(_M_end_of_storage - _M_finish) >= n) {
            T x_copy = x;
            const size_type elems_after = _M_finish - position;
            T* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(position, old_finish - n, old_finish, __false_type());
                fill(position, position + n, x_copy);
            } else {
                __uninitialized_fill_n(_M_finish, n - elems_after, x_copy, __false_type());
                _M_finish += n - elems_after;
                __uninitialized_copy(position, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        } else
            _M_insert_overflow(position, x, __false_type(), n);
    }
}
template class vector<CScriptGameResourceObjectScriptedThingBase>;
}