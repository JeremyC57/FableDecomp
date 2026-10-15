// Behaviour test for 0x0045F716: memberwise copy of the multi-strike def's def-string arrays and width.
#include <cstdio>
struct DefString { int TablePos; };
struct Def { DefString sounds[4], anims[4]; float width;
  Def& operator=(const Def& o) { for (int i = 0; i < 4; ++i) sounds[i] = o.sounds[i]; for (int j = 0; j < 4; ++j) anims[j] = o.anims[j]; width = o.width; return *this; } };
int main()
{
    Def a, b; for (int i = 0; i < 4; ++i) { a.sounds[i].TablePos = i; a.anims[i].TablePos = 10 + i; b.sounds[i].TablePos = b.anims[i].TablePos = -1; } a.width = 3.0f; b.width = 0;
    Def* r = &(b = a);
    bool ok = r == &b && b.sounds[2].TablePos == 2 && b.anims[3].TablePos == 13 && b.width == 3.0f;
    if (!ok) { std::printf("0045f716_TEST FAIL\n"); return 1; }
    std::printf("0045f716_TEST PASS\n");
    return 0;
}
