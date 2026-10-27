// behaviour test for 0x00776410: != compares the object with the pointer's current item
#include <cstdio>
struct Obj { int x; };
struct IP { Obj* item; Obj* GetPItem() const { return item; } };
static bool NotEqual(const Obj* o, const IP& p) { return o != p.GetPItem(); }
int main()
{
    Obj a, b; IP p = { &a }; IP n = { 0 };
    if (NotEqual(&a, p) || !NotEqual(&b, p) || NotEqual(0, n) || !NotEqual(&a, n)) { std::printf("00776410_TEST FAIL\n"); return 1; }
    std::printf("00776410_TEST PASS\n");
    return 0;
}