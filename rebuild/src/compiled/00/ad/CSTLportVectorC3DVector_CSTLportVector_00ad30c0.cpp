// CSTLportVector<C3DVector>::CSTLportVector(unsigned long count, const C3DVector& value, const allocator&) -- STLport vector of 12-byte vectors: pointers nulled, count elements malloc'd (none for zero), finish/end-of-storage set, every element copy-constructed from value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD30C0, length 116 bytes.
// Leaf: ??0?$CSTLportVector@
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

template <class T> __forceinline T* UninitializedFillN(T* first, unsigned long count, const T& value)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor)
        new (cursor) T(value);
    return cursor;
}

template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
    T* allocate(unsigned long count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};

template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;                    // +0
    T*             m_pFinish;                   // +4
    CAllocProxy<T> m_endOfStorage;              // +8
    CVectorBase(unsigned long count, const CAllocator<T>& alloc)
        : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0)
    {
        m_pStart = m_endOfStorage.allocate(count);
        m_pFinish = m_pStart;
        m_endOfStorage.m_data = m_pStart + count;
    }
};

template <class T> struct CSTLportVector : CVectorBase<T> {     // STLport vector<T>
    explicit CSTLportVector(unsigned long count)
        : CVectorBase<T>(count, CAllocator<T>())
    {
        this->m_pFinish = UninitializedFillN(this->m_pStart, count, T());
    }
    CSTLportVector(unsigned long count, const T& value, const CAllocator<T>& alloc = CAllocator<T>())
        : CVectorBase<T>(count, alloc)
    {
        this->m_pFinish = UninitializedFillN(this->m_pStart, count, value);
    }
};

template CSTLportVector<C3DVector>::CSTLportVector(unsigned long, const C3DVector&, const CAllocator<C3DVector>&);