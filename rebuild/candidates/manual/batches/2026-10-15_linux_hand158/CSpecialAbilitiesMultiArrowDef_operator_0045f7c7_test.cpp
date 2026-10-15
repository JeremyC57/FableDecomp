// Behaviour test for 0x0045F7C7: memberwise copy of the multi-arrow def's arrays and scalars.
#include <cstdio>
struct Def { int shots[4], volley[4]; float delay; int sound; float width;
  Def& operator=(const Def& o) { for (int i = 0; i < 4; ++i) shots[i] = o.shots[i]; for (int j = 0; j < 4; ++j) volley[j] = o.volley[j]; delay = o.delay; sound = o.sound; width = o.width; return *this; } };
int main()
{
    Def a = {{1, 2, 3, 4}, {5, 6, 7, 8}, 0.5f, 9, 2.0f}, b = {{0}, {0}, 0, 0, 0};
    Def* r = &(b = a);
    bool ok = r == &b && b.shots[3] == 4 && b.volley[0] == 5 && b.volley[3] == 8 && b.delay == 0.5f && b.sound == 9 && b.width == 2.0f;
    if (!ok) { std::printf("0045f7c7_TEST FAIL\n"); return 1; }
    std::printf("0045f7c7_TEST PASS\n");
    return 0;
}
