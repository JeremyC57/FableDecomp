#include <cstdio>
struct P { void* v; void* GetPItem() { return v; } };
struct T { unsigned char pad[0x10]; P ptr; bool Avail() { return ptr.GetPItem() == 0; } };
int main()
{
    T t; t.ptr.v = 0; if (!t.Avail()) { std::printf("0065edac_TEST FAIL\n"); return 1; }
    t.ptr.v = &t; if (t.Avail()) { std::printf("0065edac_TEST FAIL\n"); return 1; }

    std::printf("0065edac_TEST PASS\n");
    return 0;
}