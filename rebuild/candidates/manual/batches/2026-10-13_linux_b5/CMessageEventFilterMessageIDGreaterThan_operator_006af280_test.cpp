#include <cstdio>
struct E { unsigned char pad[0x40]; unsigned long id; };
struct F { unsigned long id; bool operator()(E* e) { return id < e->id; } };
int main()
{
    E e; e.id = 10; F f; f.id = 5; if (!f(&e)) { std::printf("006af280_TEST FAIL\n"); return 1; }
    f.id = 10; if (f(&e)) { std::printf("006af280_TEST FAIL\n"); return 1; }

    std::printf("006af280_TEST PASS\n");
    return 0;
}
