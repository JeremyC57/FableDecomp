#include <cstdio>
// Behaviour model: the host operation runs only for a matching key.
static void* applied = 0;
struct K { long key; };
struct H { long key; void Op(void* p) { applied = p; } };
struct Cb { K* held; void Invoke(H* h) { K* k = held; if (h->key == k->key) h->Op(k); } };
int main()
{
    K k = { 7 }; Cb cb = { &k }; H other = { 8 }, same = { 7 };
    cb.Invoke(&other);
    if (applied) { std::printf("00c2bab0_TEST FAIL\n"); return 1; }
    cb.Invoke(&same);
    if (applied != &k) { std::printf("00c2bab0_TEST FAIL\n"); return 1; }
    std::printf("00c2bab0_TEST PASS\n");
    return 0;
}