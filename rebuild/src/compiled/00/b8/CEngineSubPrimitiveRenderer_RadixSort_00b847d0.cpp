// CEngineSubPrimitiveRenderer::RadixSort(CArray<CSortElement>& elements) -- three-pass LSD radix sort (10 bits per pass) of the sort elements by key, ping-ponging through the renderer's scratch array, the result copied back when it ends in the scratch
// Leaf: ?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B847D0, 444 bytes (448 with padding), `ret 4`; Ego_r.pdb / FableWin.pdb
//   ?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z (private).
// Callees: _chkstk 0x00BFEA30 (0x201c-byte frame), the scratch vector's erase(first, last) 0x00B85AE0 and
//   insert(pos, count, const T&) 0x00B85FD0 (STLport resize, inlined).
// Layout: engine/CEngineSubPrimitiveRenderer.h retail branch + nested opt-in FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RADIXSORT
//   (CSortElement, SortBuffer CArray<CSortElement> at +0x180); CArray::GetAsCArray is `size() > 0 ? &front() : (T*)this`.
// Fix (chunk 762): the pass loop counts passes (shift = pass * 10, strength-reduced back to shift += 10), which places the
//   shift's `xor ebx,ebx` after the hoisted prefix-sum pointers; the radix is a named local in the histogram and scatter
//   loops, which gives the histogram its own index register (`xor edx,edx`) and retail's load-then-lea increments.
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RADIXSORT
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#include "engine/CEngineSubPrimitiveRenderer.h"
#include <string.h>

void CEngineSubPrimitiveRenderer::RadixSort(CArray<CSortElement>& elements)
{
    unsigned long count = elements.size();
    SortBuffer.resize(count);
    CSortElement* src = elements.GetAsCArray();
    CSortElement* dst = SortBuffer.GetAsCArray();
    unsigned long offsets[1024];
    unsigned long counts[1024];
    for (unsigned int pass = 0; pass < 3; ++pass) {
        unsigned long shift = pass * 10;
        memset(counts, 0, sizeof(counts));
        unsigned long i;
        for (i = 0; i < count; ++i) {
            unsigned long radix = (src[i].Key >> shift) & 0x3ff;
            counts[radix]++;
        }
        offsets[0] = 0;
        for (unsigned long j = 1; j < 1024; ++j)
            offsets[j] = offsets[j - 1] + counts[j - 1];
        for (i = 0; i < count; ++i) {
            unsigned long radix = (src[i].Key >> shift) & 0x3ff;
            unsigned long index = offsets[radix]++;
            dst[index] = src[i];
        }
        CSortElement* swap = src;
        src = dst;
        dst = swap;
    }
    if (src != elements.GetAsCArray())
        memcpy(elements.GetAsCArray(), src, elements.size() * sizeof(CSortElement));
}