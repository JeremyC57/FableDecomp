// CSTLportVector<C3DVector>::CSTLportVector(unsigned long count) -- STLport vector of 12-byte vectors: pointers nulled, count elements malloc'd (none for zero), finish/end-of-storage set, every element copy-constructed from an uninitialised T()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C2AB0, length 115 bytes.
// Leaf: ??0?$CSTLportVector@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Callees: malloc @ 0x00BFEA0E (`__cdecl`).
// Label: the manifest's "CSimpleMemoryPool::New<C3DAnimationSequence>" is propagated (as on the landed twin 0x009D2AF0):
// `this` is the vector itself, the body STLport's _Vector_base(n, alloc) + uninitialized_fill_n inlined.
// Model: the landed 0x009D2AF0 STLport _Vector_base / _STLP_alloc_proxy model with a 12-byte element whose default ctor is
// empty (T() leaves the frame temporary unset); UninitializedFillN must be `__forceinline` here.
// Shape: the element has a user memberwise copy ctor -- the zero initialisers stay immediates and the loop re-reads the
// three words of the T() temporary every pass (the implicit copy hoists them into ebx/edx/ecx: that is 0x008F69B0).
#include "fable_compiler.h"
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

#include "engine/C3DVector.h"

template <class T> FABLE_FORCEINLINE T* UninitializedFillN(T* first, unsigned long count, const T& value)
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

template struct CSTLportVector<C3DVector>;