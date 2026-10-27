// behaviour test for 0x004D6C66: the copy constructor copies all three members
#include <cstdio>
#include <string>
struct R { void* f; unsigned long t; std::string n; };
int main()
{
    int x; R a; a.f = &x; a.t = 7; a.n = "name";
    R b(a); R c; c = a;
    int ok = b.f == &x && b.t == 7 && b.n == "name" && c.f == &x && c.t == 7 && c.n == "name";
    if (!ok) { std::printf("004d6c66_TEST FAIL\n"); return 1; }
    std::printf("004d6c66_TEST PASS\n");
    return 0;
}