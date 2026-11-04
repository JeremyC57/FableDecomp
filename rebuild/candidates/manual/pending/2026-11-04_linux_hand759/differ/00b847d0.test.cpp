// Behaviour test for CEngineSubPrimitiveRenderer::RadixSort (0x00B847D0, parked DIFFER): a model of the row -- three 10-bit LSD passes sort the elements by key.
#include <stdio.h>
#include <vector>
struct E { unsigned long key; int id; };
static void RadixSort(std::vector<E>& v)
{ std::vector<E> tmp(v.size());
  for (int pass = 0; pass < 3; ++pass) { unsigned long count[1025] = { 0 }; int sh = pass * 10;
    for (size_t i = 0; i < v.size(); ++i) ++count[((v[i].key >> sh) & 1023) + 1];
    for (int i = 0; i < 1024; ++i) count[i + 1] += count[i];
    for (size_t i = 0; i < v.size(); ++i) tmp[count[(v[i].key >> sh) & 1023]++] = v[i];
    v.swap(tmp); } }
int main() { std::vector<E> v; unsigned long k[5] = { 900000, 3, 70000, 3, 1024 };
  for (int i = 0; i < 5; ++i) { E e = { k[i], i }; v.push_back(e); } RadixSort(v);
  bool ok = v[0].key == 3 && v[0].id == 1 && v[1].id == 3 && v[2].key == 1024 && v[3].key == 70000 && v[4].key == 900000;
  printf(ok ? "00b847d0_TEST PASS\n" : "00b847d0_TEST FAIL\n");
  return ok ? 0 : 1;
}
