// std::vector::_M_insert_overflow(COptimisedSubMesh* position, const COptimisedSubMesh& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the optimised mesh's sub-mesh vector into a size + max(size, n) block with x inserted fill_len times at position
// Leaf: ?_M_insert_overflow@?$vector@VCOptimisedSubMesh@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD83D0, 243 bytes, thiscall `ret 14h`.
// Identity: the 0x18-byte element (`imul 2AAAAAABh ; sar edx,2`) is QuadricOptimiserInternals::COptimisedSubMesh
//   { vector<CCountedPointer<COptimisedPrimitive> > Primitives; vector<COptimisedBone> Bones; } -- the fill_len == 1
//   copy calls the landed vector copy constructors 0x00AD26D0 (Primitives) and 0x00AD2730 (Bones at +0xC), and the
//   helpers are the landed COptimisedSubMesh __uninitialized_copy 0x00AD75F0, __uninitialized_fill_n 0x00AD76E0 and
//   __destroy_aux 0x00AD7DC0. Callers 0x00ACEB70 / 0x00AD8764 (push_back / insert). The catalog's
//   "Vector_ReallocateElements_6Way" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 and the five above. 0x00AD75F0 is reached only from here, so it is the
//   <COptimisedSubMesh*, COptimisedSubMesh*> instantiation (STLport passes `_M_start` / `position`, non-const).
// Shape: STLport 4.6 `_M_insert_overflow(.., const __false_type&, ..)` verbatim with the copy/fill/destroy helpers as
//   same-TU noinline bodies (each `__false_type()` tag homed on the `atend` slot; declaration-only helpers give the
//   first tag its own frame byte), COptimisedSubMesh's implicit copy constructor
//   inline (two vector copy-constructor calls), _M_clear = __destroy_aux + `if (start) free(start)`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
};

class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// The helpers' real bodies (landed 0x00AD75F0 / 0x00AD76E0 / 0x00AD7DC0) sit in this TU as in retail; out of line
// (auto_inline off + noinline) they let VC7.1 home each __false_type() tag on the dead `atend` slot. Not graded here.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AD75F0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                              // 0x00AD76E0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                              // 0x00AD7DC0
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
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    vector(const vector<_Tp, _Alloc>& __x);               // out of line (0x00AD26D0 / 0x00AD2730)
    ~vector();                                            // out of line
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

}   // namespace std

namespace QuadricOptimiserInternals {

class COptimisedPrimitive;

class COptimisedBone {                                      // 16 bytes
public:
    CCharString BoneName;                                   // +0x0
    long        Parent;                                     // +0x4
    bool        Remapped;                                   // +0x8
    long        NewIndex;                                   // +0xC
};

class COptimisedSubMesh {                                   // 0x18 bytes in retail
public:
    std::vector<CCountedPointer<COptimisedPrimitive> > Primitives;   // +0x00
    std::vector<COptimisedBone>                        Bones;        // +0x0C
};

}   // namespace QuadricOptimiserInternals

template void std::vector<QuadricOptimiserInternals::COptimisedSubMesh>::_M_insert_overflow(
    QuadricOptimiserInternals::COptimisedSubMesh*, const QuadricOptimiserInternals::COptimisedSubMesh&,
    const std::__false_type&, unsigned int, bool);