#include <cstdio>
// Behaviour model: the member is cached only when the component exists.
int main()
{
    int comp = 5; int* member = 0; bool has = true;
    if (has) member = &comp;
    if (member != &comp) { std::printf("00940b10_TEST FAIL\n"); return 1; }
    std::printf("00940b10_TEST PASS\n");
    return 0;
}