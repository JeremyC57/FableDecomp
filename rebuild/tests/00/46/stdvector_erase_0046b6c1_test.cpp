// behaviour test for 0x0046B6C1: erasing a middle range shifts the tail down and returns first
#include <cstdio>
struct E { unsigned int w[4]; };
struct V { E* s; E* f;
    E* erase(E* first, E* last) { E* r = first; for (long n = (long)(f - last); n > 0; --n) *r++ = *last++; f = r; return first; } };
int main()
{
    E buf[6]; for (int i = 0; i < 6; ++i) buf[i].w[0] = i;
    V v; v.s = buf; v.f = buf + 6;
    E* r = v.erase(buf + 1, buf + 3);
    if (r != buf + 1 || v.f != buf + 4 || buf[1].w[0] != 3 || buf[3].w[0] != 5) { std::printf("0046b6c1_TEST FAIL\n"); return 1; }
    std::printf("0046b6c1_TEST PASS\n");
    return 0;
}