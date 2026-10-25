// behaviour test for 0x00472220 std::vector<EOpinionAttitudeType >::_M_fill_insert: inserting n copies of x at pos keeps the order
// of the old elements around the new run, both when the tail is longer than n and when it is not; n == 0 is a no-op;
// a full vector goes to the overflow path
#include <cstdio>
struct Vec { int* start; int* finish; int* eos; };
static int gOverflow = 0;
static void FillInsert(Vec& v, int* pos, unsigned int n, const int& x)
{
    if (n == 0) return;
    if ((unsigned int)(v.eos - v.finish) >= n) {
        int xc = x;
        unsigned int after = (unsigned int)(v.finish - pos);
        int* oldFinish = v.finish;
        if (after > n) {
            for (int* s = v.finish - n, *d = v.finish; s != v.finish; ++s, ++d) *d = *s;
            v.finish += n;
            for (int* s = oldFinish - n, *d = oldFinish; s != pos; ) *--d = *--s;
            for (int* p = pos; p != pos + n; ++p) *p = xc;
        } else {
            for (unsigned int k = 0; k < n - after; ++k) v.finish[k] = xc;
            v.finish += n - after;
            for (int* s = pos, *d = v.finish; s != oldFinish; ++s, ++d) *d = *s;
            v.finish += after;
            for (int* p = pos; p != oldFinish; ++p) *p = xc;
        }
    } else ++gOverflow;
}
int main()
{
    int fails = 0;
    int buf[10] = { 1, 2, 3, 4, 0, 0, 0, 0, 0, 0 };
    Vec v; v.start = buf; v.finish = buf + 4; v.eos = buf + 10;
    FillInsert(v, buf + 1, 2, 9);                      // tail 3 > 2
    int e1[6] = { 1, 9, 9, 2, 3, 4 };
    for (int i = 0; i < 6; ++i) if (buf[i] != e1[i]) fails |= 1;
    if (v.finish != buf + 6) fails |= 2;
    FillInsert(v, buf + 5, 3, 7);                      // tail 1 <= 3
    int e2[9] = { 1, 9, 9, 2, 3, 7, 7, 7, 4 };
    for (int i = 0; i < 9; ++i) if (buf[i] != e2[i]) fails |= 4;
    FillInsert(v, buf, 0, 5);
    if (v.finish != buf + 9 || buf[0] != 1) fails |= 8;
    FillInsert(v, buf, 2, 5);
    if (gOverflow != 1) fails |= 16;
    if (fails) { std::printf("00472220_TEST FAIL %d\n", fails); return 1; }
    std::printf("00472220_TEST PASS\n");
    return 0;
}