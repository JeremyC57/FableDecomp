// behaviour test for 0x0046C4A4 std::vector<CObjectFamilyEntry >::_M_fill_insert: inserting n copies of an 8-byte x at pos keeps the order of the old
// elements around the new run, both when the tail is longer than n and when it is not; n == 0 is a no-op; a full vector goes to the overflow path
#include <cstdio>
struct E { int a, b; };
struct Vec { E* start; E* finish; E* eos; };
static int gOverflow = 0;
static void FillInsert(Vec& v, E* pos, unsigned int n, const E& x)
{
    if (n == 0) return;
    if ((unsigned int)(v.eos - v.finish) >= n) {
        E xc = x;
        unsigned int after = (unsigned int)(v.finish - pos);
        E* oldFinish = v.finish;
        if (after > n) {
            for (E* s = v.finish - n, *d = v.finish; s != v.finish; ++s, ++d) *d = *s;
            v.finish += n;
            for (E* s = oldFinish - n, *d = oldFinish; s != pos; ) *--d = *--s;
            for (E* p = pos; p != pos + n; ++p) *p = xc;
        } else {
            for (unsigned int k = 0; k < n - after; ++k) v.finish[k] = xc;
            v.finish += n - after;
            for (E* s = pos, *d = v.finish; s != oldFinish; ++s, ++d) *d = *s;
            v.finish += after;
            for (E* p = pos; p != oldFinish; ++p) *p = xc;
        }
    } else ++gOverflow;
}
static E Mk(int a) { E e; e.a = a; e.b = -a; return e; }
int main()
{
    int fails = 0;
    E buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = Mk(i < 4 ? i + 1 : 0);
    Vec v; v.start = buf; v.finish = buf + 4; v.eos = buf + 10;
    FillInsert(v, buf + 1, 2, Mk(9));                      // tail 3 > 2
    int e1[6] = { 1, 9, 9, 2, 3, 4 };
    for (int i = 0; i < 6; ++i) if (buf[i].a != e1[i] || buf[i].b != -e1[i]) fails |= 1;
    if (v.finish != buf + 6) fails |= 2;
    FillInsert(v, buf + 5, 3, Mk(7));                      // tail 1 <= 3
    int e2[9] = { 1, 9, 9, 2, 3, 7, 7, 7, 4 };
    for (int i = 0; i < 9; ++i) if (buf[i].a != e2[i] || buf[i].b != -e2[i]) fails |= 4;
    FillInsert(v, buf, 0, Mk(5));
    if (v.finish != buf + 9 || buf[0].a != 1) fails |= 8;
    FillInsert(v, buf, 2, Mk(5));
    if (gOverflow != 1) fails |= 16;
    if (fails) { std::printf("0046c4a4_TEST FAIL %d\n", fails); return 1; }
    std::printf("0046c4a4_TEST PASS\n");
    return 0;
}