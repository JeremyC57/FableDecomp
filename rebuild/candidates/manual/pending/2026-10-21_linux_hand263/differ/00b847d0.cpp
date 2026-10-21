// Grade: DIFFER(444v444) -- register allocation only: retail gives the histogram loop its own zero (`xor edx,edx` index, key in eax) and inits the shift after the pointer setup; ours reuses the memset zero eax as the index (tried ++/+=/post-inc, signed/int/scoped index, while loop, clear loop, GetRadix helper)
// CEngineSubPrimitiveRenderer::RadixSort(CArray<CSortElement>& elements) -- three-pass LSD radix sort (10 bits per pass) of the sort elements by key, ping-ponging through the renderer's scratch array, the result copied back when it ends in the scratch
// Leaf: ?RadixSort@CEngineSubPrimitiveRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B847D0, length 448 bytes (body 0x1bc + int3 padding), `ret 4`.
// Callees: _chkstk 0x00BFEA30 (0x201c-byte frame), the scratch CArray's erase(first, last) 0x00B85AE0 and
//   insert(pos, count, const T&) 0x00B85FD0 (STLport resize, inlined).
// Layout: the scratch CArray<CSortElement> (STLport vector, 12 bytes) sits at +0x180 of `this` (no engine header
//   models CEngineSubPrimitiveRenderer); CSortElement is 8 bytes with the key in the first dword.
// Shape: CArray::GetData() is `size() > 0 ? start : (T*)this` (the landed CMemoryDataInputStream model); per pass the
//   counts are cleared, histogrammed, prefix-summed into the offsets, and the elements scattered.
#include <string.h>

struct CSortElement {                              // CEngineSubPrimitiveRenderer::CSortElement, 8 bytes
    unsigned long Key;                             // +0x00
    void*         Primitive;                       // +0x04
    CSortElement() {}
};

struct CArraySortElement {                         // CArray<CSortElement> (STLport vector)
    CSortElement* m_start;
    CSortElement* m_finish;
    CSortElement* m_endOfStorage;
    unsigned long size() const { return m_finish - m_start; }
    CSortElement* GetData() { return size() > 0 ? m_start : (CSortElement*)this; }
    CSortElement* erase(CSortElement* first, CSortElement* last);                   // 0x00B85AE0
    void insert(CSortElement* pos, unsigned long count, const CSortElement& value); // 0x00B85FD0
    void resize(unsigned long newSize, const CSortElement& value = CSortElement())
    {
        if (newSize < size())
            erase(m_start + newSize, m_finish);
        else
            insert(m_finish, newSize - size(), value);
    }
};

struct CEngineSubPrimitiveRenderer {
    unsigned char     _pad000[0x180];
    CArraySortElement SortScratch;                 // +0x180
    void RadixSort(CArraySortElement& elements);
};

void CEngineSubPrimitiveRenderer::RadixSort(CArraySortElement& elements)
{
    unsigned long count = elements.size();
    SortScratch.resize(count);
    CSortElement* src = elements.GetData();
    CSortElement* dst = SortScratch.GetData();
    unsigned long offsets[1024];
    unsigned long counts[1024];
    for (unsigned long shift = 0; shift < 30; shift += 10) {
        memset(counts, 0, sizeof(counts));
        unsigned long i;
        for (i = 0; i < count; ++i)
            ++counts[(src[i].Key >> shift) & 0x3ff];
        offsets[0] = 0;
        for (unsigned long j = 1; j < 1024; ++j)
            offsets[j] = offsets[j - 1] + counts[j - 1];
        for (i = 0; i < count; ++i) {
            unsigned long index = offsets[(src[i].Key >> shift) & 0x3ff]++;
            dst[index] = src[i];
        }
        CSortElement* swap = src;
        src = dst;
        dst = swap;
    }
    if (src != elements.GetData())
        memcpy(elements.GetData(), src, elements.size() * sizeof(CSortElement));
}
