#include <cstdio>
#include <vector>
// Behaviour model: resize shrinks by erasing the tail, grows by appending value-initialised (zero) elements.
struct E { long a, b; };
int main()
{
    std::vector<E> v(3);
    for (int i = 0; i < 3; ++i) { v[i].a = i + 1; v[i].b = 9; }
    v.resize(1);
    if (v.size() != 1 || v[0].a != 1) { std::printf("007b5210_TEST FAIL\n"); return 1; }
    v.resize(4, E());
    if (v.size() != 4 || v[3].a != 0 || v[3].b != 0 || v[0].b != 9) { std::printf("007b5210_TEST FAIL\n"); return 1; }
    v.resize(0);
    if (!v.empty()) { std::printf("007b5210_TEST FAIL\n"); return 1; }
    std::printf("007b5210_TEST PASS\n");
    return 0;
}