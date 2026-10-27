#include <cstdio>
#include <cstring>
// Behaviour model: the two cleared members read zero after construction, the others are untouched.
struct T { void* a; unsigned int b; void* c; T() { std::memset(this, 0x5a, sizeof *this); a = 0; c = 0; } };
int main()
{
    T t;
    if (t.a || t.c || t.b != 0x5a5a5a5aU) { std::printf("00c39d30_TEST FAIL\n"); return 1; }
    std::printf("00c39d30_TEST PASS\n");
    return 0;
}