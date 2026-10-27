// behaviour test for 0x004D43B9: a new component starts with every member cleared
#include <cstdio>
struct Base { void* thing; Base(void* t) : thing(t) { } };
struct C : Base { long m[4]; C(void* t) : Base(t) { for (int i = 0; i < 4; ++i) m[i] = 0; } };
int main()
{
    int thing = 0; C c(&thing);
    int ok = c.thing == &thing && !c.m[0] && !c.m[1] && !c.m[2] && !c.m[3];
    if (!ok) { std::printf("004d43b9_TEST FAIL\n"); return 1; }
    std::printf("004d43b9_TEST PASS\n");
    return 0;
}