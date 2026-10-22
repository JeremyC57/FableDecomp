#include <cstdio>
struct O {}; static int g_c = 0; struct X { unsigned char body[0x2c]; X(O* o) { ++g_c; body[0] = o ? 1 : 0; } };
static X* __fastcall Create(O* o) { return new X(o); }
int main()
{
    O o; X* x = Create(&o); if (!x || g_c != 1 || x->body[0] != 1) { std::printf("004e7e19_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("004e7e19_TEST PASS\n");
    return 0;
}