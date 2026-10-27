#include <cstdio>
#include <vector>
// Behaviour model: clear empties the vector and keeps its storage.
struct E { long a; float b; };
int main()
{
    std::vector<E> v(5);
    unsigned cap = v.capacity();
    v.clear();
    if (!v.empty() || v.capacity() != cap) { std::printf("007b4bb0_TEST FAIL\n"); return 1; }
    v.clear();
    if (!v.empty()) { std::printf("007b4bb0_TEST FAIL\n"); return 1; }
    std::printf("007b4bb0_TEST PASS\n");
    return 0;
}