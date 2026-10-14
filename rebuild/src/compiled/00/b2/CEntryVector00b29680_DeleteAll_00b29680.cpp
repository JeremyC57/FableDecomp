// CEntryVector_00b29680::DeleteAll() -- delete every non-null entry of this pointer vector (its virtual destructor, slot 0 with 1), then drop them all: the VC6-era `erase(begin(), end())` (the tail copy a memmove of `end - end` bytes, the compare of the register with itself kept)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29680, length 84 bytes.
// Leaf: DeleteAll
// Callees: memmove @ 0x00BFEAE6. The chunk label (CMeshDataBank::FreeDeviceSpecificResources) is propagated; named by address. Shape: `this` in esi, the index in edi, `ret`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);

template<class T> struct CRetailVector {           // VC6-era Dinkumware vector: first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    __forceinline T* Copy(T* first, T* last, T* dest)   // the POD `copy` of the erase: a memmove of the tail
    {
        if (first == last)
            return dest;
        long bytes = (char*)last - (char*)first;
        return (T*)((char*)memmove(dest, first, bytes) + bytes);
    }
    __forceinline void erase(T* first, T* last)
    {
        T* kept = Copy(last, m_last, first);
        m_last = kept;
    }
};

struct CEntry_00b29680 {
    virtual ~CEntry_00b29680();
};

struct CEntryVector_00b29680 : CRetailVector<CEntry_00b29680*> {
    void DeleteAll();
};

void CEntryVector_00b29680::DeleteAll()
{
    for (unsigned long i = 0; i < (unsigned long)(m_last - m_first); ++i) {
        if (m_first[i])
            delete m_first[i];
    }
    erase(m_first, m_last);
}