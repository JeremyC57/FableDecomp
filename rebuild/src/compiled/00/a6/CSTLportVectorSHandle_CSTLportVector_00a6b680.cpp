// CSTLportVector<SHandle>::CSTLportVector(unsigned long count) -- STLport vector(n) of 4-byte handles: the three pointers nulled, count elements allocated (none for zero), finish / end-of-storage set, then every element copy-constructed from a zero handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B680, length 93 bytes.
// Leaf: ??0?$CSTLportVector@
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// The manifest label "CSimpleMemoryPool::New<C3DAnimationSequence>" is propagated: `this` is the vector itself (three
// words), the body is STLport's _Vector_base(n) + uninitialized_fill_n(start, n, T()) inlined; 0x00B2EC80 is a
// byte-identical instantiation for another 4-byte element type.
// Shape: STLport's real structure is needed -- _Vector_base(n, allocator_type()) with the end-of-storage pointer in an
// _STLP_alloc_proxy over the EMPTY allocator (a plain three-pointer class schedules `mov esi,ecx` after the count test);
// the base's null initialisers stored as immediates, `allocate(n)` = `n != 0 ? malloc(n * 4) : 0`, the
// null-checked placement-copy loop storing the zero value as an immediate, finish stored from the cursor.
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct SHandle {                                // 4 bytes, cleared by the default constructor
    long m_value;
    SHandle() : m_value(0) { }
};

template <class T> T* UninitializedFillN(T* first, unsigned long count, const T& value)
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
};

template struct CSTLportVector<SHandle>;