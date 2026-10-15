// CRandomAppearanceMorph::UninitializedFillN(CMorphRecord* first, unsigned long n, const CMorphRecord& x, __false_type) -- STLport __uninitialized_fill_n over a 28-byte morph record (four words plus a vector of 16-byte entries); returns the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE710, length 155 bytes (`__fastcall`: ecx = first, edx = n;
// `ret 8`: the value reference and the by-value tag). Called from the vector reallocation at 0x00BEF8D6
// (catalog "CRandomAppearanceMorph_Reallocate"); the catalog name "CopyBodyPartMeshes" is descriptive only.
// Callees: malloc @ 0x00BFEA0E (the allocator), the inner __uninitialized_copy of the 16-byte entries @ 0x005BAF28
// (`__fastcall` first/last in ecx/edx, then result and the tag reference).
// Shape: STLport's fill loop (`for (; n > 0; --n, ++cur) _Construct(&*cur, x)`, null-checked placement new) over
// the record's implicit copy constructor: four dwords, then the STLport vector copy constructor (_Vector_base(n)
// with the alloc proxy of the landed 0x00B2EC80, then the inner uninitialized copy with a fresh _IsPODType() tag,
// which VC places in the dead value-argument slot `[esp+18h]`). The four leading words must be separate members
// (an array member copies through `mov ecx,ebx ; mov edx,ebp` pointers).
// Leaf: UninitializedFillN
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct __false_type { __false_type() {} };

struct CMorphEntry {                            // 16-byte element of the inner vector (copied by 0x005BA48A)
    unsigned long m_words[4];
    CMorphEntry(const CMorphEntry& other);
};

CMorphEntry* __fastcall UninitializedCopy(const CMorphEntry* first, const CMorphEntry* last, CMorphEntry* result,
                                          const __false_type& tag);   // 0x005BAF28

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
    unsigned long size() const { return (unsigned long)(this->m_pFinish - this->m_pStart); }
    CSTLportVector(const CSTLportVector& other)
        : CVectorBase<T>(other.size(), CAllocator<T>())
    {
        this->m_pFinish = UninitializedCopy(other.m_pStart, other.m_pFinish, this->m_pStart, __false_type());
    }
};

struct CMorphRecord {                           // 0x1c bytes
    unsigned long               m_word0;        // +0x00
    unsigned long               m_word4;        // +0x04
    unsigned long               m_word8;        // +0x08
    unsigned long               m_wordC;        // +0x0c
    CSTLportVector<CMorphEntry> m_entries;      // +0x10
};

CMorphRecord* __fastcall UninitializedFillN(CMorphRecord* first, unsigned long n, const CMorphRecord& x, __false_type)
{
    CMorphRecord* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CMorphRecord(x);
    return cur;
}
