// behaviour test for 00b8f180 (standalone copy of the model)
#include <cstdio>

struct Pr { unsigned long first; unsigned long second; };
struct MapperPred { bool operator()(const Pr& e, unsigned long id) const { return e.first < id; } };
Pr* lower_bound_(Pr* first, Pr* last, const unsigned long& val, MapperPred comp)
{
    long len = (long)(last - first);
    while (len > 0) { long half = len >> 1; Pr* mid = first + half;
        if (comp(*mid, val)) { first = mid + 1; len = len - half - 1; } else len = half; }
    return first;
}

int main()
{
    Pr v[5] = { {1,0}, {3,0}, {3,1}, {7,0}, {9,0} }; MapperPred p;
    unsigned long k0 = 0, k3 = 3, k4 = 4, k10 = 10;
    if (!(lower_bound_(v, v + 5, k0, p) == v && lower_bound_(v, v + 5, k3, p) == v + 1 && lower_bound_(v, v + 5, k4, p) == v + 3 && lower_bound_(v, v + 5, k10, p) == v + 5 && lower_bound_(v, v, k3, p) == v)) { std::printf("00b8f180_TEST FAIL\n"); return 1; }
    std::printf("00b8f180_TEST PASS\n");
    return 0;
}