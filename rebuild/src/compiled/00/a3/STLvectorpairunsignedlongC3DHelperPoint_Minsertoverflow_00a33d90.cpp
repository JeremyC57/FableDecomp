// std::vector<pair<unsigned long,C3DHelperPoint> >::_M_insert_overflow(pair<unsigned long, C3DHelperPoint>* position, const pair<unsigned long, C3DHelperPoint>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the helper-point vector: allocate max(2*size, size+fill_len) slots, copy the head, fill_len copies of x and (unless appending) the tail, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33D90, 232 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: the regrowth of CVectorMap<unsigned long, C3DHelperPoint> (C3DMeshStats::HelperPoints); called by its Insert
//   0x00A34950 and two inlined push_backs (0x00A2F95F, 0x00A346A4). The catalog's
//   "Vector_Reallocate_C3DAnimationStateAlpha" is propagated. Callees: malloc @ 0x00BFEA0E, __uninitialized_copy(first,
//   last, result, const __false_type&) @ 0x00A31FD0, __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x00A32020,
//   free @ 0x00BFEA14.
// Element: pair<unsigned long, C3DHelperPoint> (20 bytes; C3DVector has a user copy constructor, the landed 0x00A32440 model:
//   the single-element construct is five dword moves after placement new's null test).
// Shape: STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` -- `len = old_size + (max)(old_size,
//   fill_len)` through the reference-returning max (old_size spilled to the push-ecx slot), the malloc allocator's
//   `n != 0 ? malloc(n * sizeof(T)) : 0`, the __false_type temporaries homed in the atend slot, `fill_len == 1` constructing
//   in place, then _M_clear (free when non-null; the trivial destructors vanish) and _M_set. The two tag-taking callees
//   carry same-TU `__declspec(noinline)` bodies that never touch the tag, so VC7.1 homes all three `__false_type()`
//   temporaries on the live `atend` slot (the landed 0x00569E30 / GOTCHAS 0x4ADB50 lever); only _M_insert_overflow is
//   graded.
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint { public:                                // 16 bytes (Ego_r.pdb)
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter> inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
                                                                 const __false_type&)
{                                                                                      // 0x00A31FD0
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{                                                                                      // 0x00A32020
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class vector {
public:
    T*           _M_start;                             // +0x00
    T*           _M_finish;                            // +0x04
    T*           _M_end_of_storage;                    // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        allocator<T>().deallocate(_M_start, _M_end_of_storage - _M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = allocator<T>().allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<unsigned long, C3DHelperPoint> >;

}