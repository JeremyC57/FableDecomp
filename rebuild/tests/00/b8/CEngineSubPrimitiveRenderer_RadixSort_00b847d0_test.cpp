// Behaviour test for CEngineSubPrimitiveRenderer::RadixSort (0x00B847D0): a model of the row -- three 10-bit LSD passes (histogram, prefix sum, stable scatter) ping-pong between the elements and the scratch buffer; after the odd pass count the result lands in the scratch and is copied back.
#include <stdio.h>
#include <string.h>
#include <vector>
struct E { unsigned long Key; int id; };
static void RadixSort(std::vector<E>& elements, std::vector<E>& scratch)
{
    unsigned long count = elements.size();
    scratch.resize(count);
    E* src = &elements[0]; E* dst = &scratch[0];
    static unsigned long offsets[1024], counts[1024];
    for (unsigned int pass = 0; pass < 3; ++pass) {
        unsigned long shift = pass * 10;
        memset(counts, 0, sizeof(counts));
        unsigned long i;
        for (i = 0; i < count; ++i) counts[(src[i].Key >> shift) & 0x3ff]++;
        offsets[0] = 0;
        for (unsigned long j = 1; j < 1024; ++j) offsets[j] = offsets[j - 1] + counts[j - 1];
        for (i = 0; i < count; ++i) dst[offsets[(src[i].Key >> shift) & 0x3ff]++] = src[i];
        E* swap = src; src = dst; dst = swap;
    }
    if (src != &elements[0]) memcpy(&elements[0], src, elements.size() * sizeof(E));
}
int main() { std::vector<E> v, s; unsigned long k[6] = { 900000, 3, 70000, 3, 1024, 0x3fffffff };
  for (int i = 0; i < 6; ++i) { E e = { k[i], i }; v.push_back(e); } RadixSort(v, s);
  bool ok = v[0].Key == 3 && v[0].id == 1 && v[1].id == 3 && v[2].Key == 1024 && v[3].Key == 70000 && v[4].Key == 900000
         && v[5].id == 5 && s.size() == 6;
  printf(ok ? "00b847d0_TEST PASS\n" : "00b847d0_TEST FAIL\n");
  return ok ? 0 : 1;
}