// CSTLportVector<SLinkPair>::CSTLportVector(unsigned long count) -- vector(n) of 8-byte link pairs: the three pointers nulled, count elements allocated (none for zero), finish / end-of-storage set, then every element default-constructed in place (both words zero)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B620, length 90 bytes.
// Leaf: ??0?$CSTLportVector@
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// The manifest label "CSimpleMemoryPool::New<C3DAnimationSequence>" is propagated: `this` is the vector itself (three
// words). Same _Vector_base(n) head as 0x009D2AF0, but the elements are DEFAULT-constructed (`new (p) T`), not copied
// from a T() value.
// Shape: STLport's real structure is needed -- _Vector_base(n, allocator_type()) with the end-of-storage pointer in an
// _STLP_alloc_proxy over the EMPTY allocator (a plain three-pointer class schedules `mov esi,ecx` after the count test);
// one zero register (ebx, saved) serves the base's null initialisers, the count compares (`cmp edi,ebx`), the
// placement-new null test and both element words; the loop head is aligned (`lea ebx,[ebx]` filler).
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct SLinkPair {                              // 8 bytes, both words cleared by the default constructor
    long m_first;
    long m_second;
    SLinkPair() : m_first(0), m_second(0) { }
};

template <class T> T* UninitializedConstructN(T* first, unsigned long count)
{
    T* cursor = first;
    for (; count > 0; --count, ++cursor)
        new (cursor) T;
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
        this->m_pFinish = UninitializedConstructN(this->m_pStart, count);
    }
};

template struct CSTLportVector<SLinkPair>;