// std::vector<C2DVector>::_M_allocate_and_copy(unsigned int n, const C2DVector* first, const C2DVector* last) -- STLport: malloc n 8-byte 2D vectors (none for zero), copy-construct [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046EB9E, 70 bytes, `ret 0Ch` (thiscall: n, first, last; the empty allocator `this` is unused).
// Callees: malloc @ 0x00BFEA0E.
// Identity: the catalog's "CActiveFile_CopyConstruct" is behavioural. The single caller is vector<C2DVector>::operator=
//   @ 0x0046EABD (stride 8, bitwise element assignment), which CPlayerGuiDef::operator= @ 0x0046CE2D (reached from
//   CPlayerGuiDef::Copy @ 0x0046CE28) calls for exactly three members: Ego_r.pdb CPlayerGuiDef has three vector<C2DVector>
//   (DPadRingsChargeOffsets, CombatCountOffsets, CrosshairOffsets) and no other 8-byte-element vector.
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
//   _Construct inlined (`new (p) T(value)`: the null test, then C2DVector's member-wise copy constructor -- X through a
//   free integer register, Y through `fld`/`fstp` once the registers run out). No engine header for C2DVector; local model.
// Shape: `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector { public:
    float X;                                        // +0x00
    float Y;                                        // +0x04
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace std {
struct __false_type { };

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    struct : A { T* _M_data; } _M_end_of_storage;   // +0x08
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);
};

template <class T, class A> T* vector<T, A>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last)
{
    T* result = _M_end_of_storage.allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}

template class vector<C2DVector, allocator<C2DVector> >;
}