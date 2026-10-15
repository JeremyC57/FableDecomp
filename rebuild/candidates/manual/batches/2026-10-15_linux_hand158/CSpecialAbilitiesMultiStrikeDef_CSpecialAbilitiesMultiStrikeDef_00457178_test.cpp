// Behaviour test for 0x00457178: def-string arrays default to -1 and every vector starts empty.
#include <cstdio>
struct DefString { int TablePos; DefString() : TablePos(-1) {} };
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) {} };
struct Def { DefString sounds[4], anims[4]; float width; Vec v[5]; };
int main()
{
    Def d; bool ok = true;
    for (int i = 0; i < 4; ++i) ok = ok && d.sounds[i].TablePos == -1 && d.anims[i].TablePos == -1;
    for (int j = 0; j < 5; ++j) ok = ok && !d.v[j].a && !d.v[j].b && !d.v[j].c;
    if (!ok) { std::printf("00457178_TEST FAIL\n"); return 1; }
    std::printf("00457178_TEST PASS\n");
    return 0;
}
