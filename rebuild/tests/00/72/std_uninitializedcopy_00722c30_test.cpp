#include <cstdio>
#include <new>
// Behaviour model: each record is copy-constructed in order and the end of the output is returned.
struct P { float x, y, z; const void* item; P(const P& o) : x(o.x), y(o.y), z(o.z), item(o.item) { } P() { } };
int main()
{
    P src[3]; int a, b, c;
    src[0].x = 1; src[0].item = &a; src[1].x = 2; src[1].item = &b; src[2].x = 3; src[2].item = &c;
    unsigned char raw[3 * sizeof(P)];
    P* out = (P*)raw;
    P* e = out;
    for (P* f = src; f != src + 3; ++f, ++e) new (e) P(*f);
    if (e != out + 3 || out[1].x != 2 || out[2].item != &c) { std::printf("00722c30_TEST FAIL\n"); return 1; }
    std::printf("00722c30_TEST PASS\n");
    return 0;
}