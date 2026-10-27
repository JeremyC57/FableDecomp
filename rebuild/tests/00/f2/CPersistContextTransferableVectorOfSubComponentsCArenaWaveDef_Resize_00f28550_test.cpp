#include <cstdio>
#include <vector>
// Behaviour model: Resize forwards to the wrapped vector with a default element.
struct D { long n; D() : n(0) { } };
struct W { std::vector<D>* v; void Resize(long n) { v->resize(n, D()); } };
int main()
{
    std::vector<D> vec; W w = { &vec }; w.Resize(4);
    if (vec.size() != 4 || vec[3].n != 0) { std::printf("00f28550_TEST FAIL\n"); return 1; }
    std::printf("00f28550_TEST PASS\n");
    return 0;
}