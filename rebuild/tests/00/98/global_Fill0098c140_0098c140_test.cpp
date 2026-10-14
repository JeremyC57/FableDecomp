#include <cstdio>
struct E { unsigned char b[16]; };
static void __fastcall F(E* f, E* l, const E& v) { for (; f != l; ++f) *f = v; }
int main()
{
    E v; v.b[0] = 7; v.b[15] = 9; E buf[3]; F(buf, buf + 3, v); if (buf[2].b[0] != 7 || buf[1].b[15] != 9) { std::printf("0098c140_TEST FAIL\n"); return 1; }

    std::printf("0098c140_TEST PASS\n");
    return 0;
}