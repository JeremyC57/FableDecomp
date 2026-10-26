// behaviour test for 0x0059AEAF std::vector<CDateAndTime >::erase(iterator position): erasing in the middle shifts the tail down by one element, erasing the last element only drops finish, and position comes back
#include <cstdio>
#include <cstdlib>
struct Elem { int W[7]; };                // the 28-byte element (trivially copyable)
struct Vec {
    Elem* first; Elem* finish; Elem* end_of_storage;
    Elem* erase(Elem* pos)                     // the function under test (STLport 4.6 shape)
    {
        if (pos + 1 != finish)
            for (Elem *s = pos + 1, *d = pos; s != finish; ++s, ++d) *d = *s;
        --finish;
        return pos;
    }
};
static Elem Make(int n) { Elem e; for (int i = 0; i < 7; ++i) e.W[i] = n * 100 + i; return e; }
int main()
{
    int fail = 0;
    Elem buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = Make(i);
    Vec v; v.first = buf; v.finish = buf + 5; v.end_of_storage = buf + 5;
    Elem* r = v.erase(buf + 1);
    if (r != buf + 1 || v.finish != buf + 4) fail |= 1;
    static const int want[4] = { 0, 2, 3, 4 };
    for (int i = 0; i < 4; ++i)
        for (int k = 0; k < 7; ++k)
            if (buf[i].W[k] != want[i] * 100 + k) fail |= 2;
    r = v.erase(buf + 3);                      // last element: no copy
    if (r != buf + 3 || v.finish != buf + 3 || buf[3].W[0] != 400) fail |= 4;
    r = v.erase(buf);
    if (r != buf || v.finish != buf + 2 || buf[0].W[0] != 200 || buf[1].W[0] != 300) fail |= 8;
    if (fail) { std::printf("0059aeaf_TEST FAIL %d\n", fail); return 1; }
    std::printf("0059aeaf_TEST PASS\n");
    return 0;
}