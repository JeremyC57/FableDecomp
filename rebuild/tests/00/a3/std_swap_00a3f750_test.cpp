#include <cstdio>
#include <string>
#include <utility>
#include <algorithm>
// Behaviour model: swap exchanges both halves.
int main()
{
    std::pair<std::string, long> a("A", 1), b("B", 2);
    std::swap(a, b);
    if (a.first != "B" || a.second != 2 || b.first != "A" || b.second != 1) { std::printf("00a3f750_TEST FAIL\n"); return 1; }
    std::printf("00a3f750_TEST PASS\n");
    return 0;
}