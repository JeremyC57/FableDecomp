// std::vector<C3DVector>::vector(size_type count, const C3DVector& value, const allocator<C3DVector>&) -- STLport vector of 12-byte vectors: pointers nulled, count elements malloc'd (none for zero), finish/end-of-storage set, every element copy-constructed from value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD30C0, length 116 bytes.
// Leaf: ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@IABVC3DVector@@ABV?$allocator@VC3DVector@@@1@@Z
// Naming (naming wave 21, chunk 689 NAME_FIXES): the STLport class names in namespace std (vector / _Vector_base / _STLP_alloc_proxy / allocator,
//   size_type unsigned int) replace the hand names CSTLportVector / CVectorBase / CAllocProxy / CAllocator; MeasureSegments 0x00AC9D50 (chunk 689)
//   binds this spelling. Text and bytes unchanged.
// Callees: malloc @ 0x00BFEA0E (`__cdecl`).
// Label: the manifest's "CSimpleMemoryPool::New<C3DAnimationSequence>" is propagated (as on the landed twin 0x009D2AF0):
// `this` is the vector itself, the body STLport's _Vector_base(n, alloc) + uninitialized_fill_n inlined.
// Model: the landed 0x009D2AF0 STLport _Vector_base / _STLP_alloc_proxy model with a 12-byte element whose default ctor is
// empty (T() leaves the frame temporary unset); UninitializedFillN must be `__forceinline` here.
// Shape: three stack arguments (`ret 0Ch`): STLport vector(n, val, alloc); the user memberwise copy ctor re-reads the
// value through its pointer every pass. Instantiated alone so the TU emits only this constructor.
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class C3DVector { public:                              // 12-byte element with an empty default ctor: T() leaves the frame temporary unset
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {

template <class T> __forceinline T* UninitializedFillN(T* first, unsigned int count, const T& value)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor)
        new (cursor) T(value);
    return cursor;
}

template <class T> class allocator {           // STLport allocator<T>: empty
public:
    allocator() { }
    T* allocate(unsigned int count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& alloc, _Value p) : _MaybeReboundAlloc(alloc), _M_data(p) { }
};

template <class T, class _Alloc> class _Vector_base {
public:
    T*             _M_start;                    // +0
    T*             _M_finish;                   // +4
    _STLP_alloc_proxy<T*, T, _Alloc> _M_end_of_storage;   // +8
    _Vector_base(unsigned int count, const _Alloc& alloc)
        : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0)
    {
        _M_start = _M_end_of_storage.allocate(count);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + count;
    }
};

template <class T, class _Alloc = allocator<T> > class vector : public _Vector_base<T, _Alloc> {     // STLport vector<T>
public:
    explicit vector(unsigned int count)
        : _Vector_base<T, _Alloc>(count, _Alloc())
    {
        this->_M_finish = UninitializedFillN(this->_M_start, count, T());
    }
    vector(unsigned int count, const T& value, const _Alloc& alloc = _Alloc())
        : _Vector_base<T, _Alloc>(count, alloc)
    {
        this->_M_finish = UninitializedFillN(this->_M_start, count, value);
    }
};

template vector<C3DVector>::vector(unsigned int, const C3DVector&, const allocator<C3DVector>&);

}