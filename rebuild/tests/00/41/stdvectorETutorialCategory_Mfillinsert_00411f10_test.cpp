// behaviour test for 0x00411F10 vector<ETutorialCategory>::_M_fill_insert (model of both in-place arms over a 4-byte element)
#include <cstdio>
#include <new>
struct Vec {
    int* start; int* finish; int* eos;
    void FillInsert(int* pos, unsigned int n, const int& x)
    {
        if (n == 0) return;
        int xc = x;
        unsigned int after = (unsigned int)(finish - pos); int* old_finish = finish;
        if (after > n) {
            int* d = finish; for (int* s = finish - n; s != finish; ++s, ++d) new (d) int(*s);
            finish += n;
            int* r = old_finish; for (int* l = old_finish - n; l != pos; ) *--r = *--l;
            for (int* p = pos; p != pos + n; ++p) *p = xc;
        } else {
            int* c = finish; for (unsigned int k = n - after; k > 0; --k, ++c) new (c) int(xc);
            finish += n - after;
            int* d = finish; for (int* s = pos; s != old_finish; ++s, ++d) new (d) int(*s);
            finish += after;
            for (int* p = pos; p != old_finish; ++p) *p = xc;
        }
    }
};
int main()
{
    int buf[10] = { 1, 2, 3, 4, 0, 0, 0, 0, 0, 0 };
    Vec v = { buf, buf + 4, buf + 10 };
    v.FillInsert(buf + 1, 2, 9);
    int ok = v.finish == buf + 6 && buf[0] == 1 && buf[1] == 9 && buf[2] == 9 && buf[3] == 2 && buf[5] == 4;
    v.FillInsert(buf + 5, 3, 7);
    ok = ok && v.finish == buf + 9 && buf[5] == 7 && buf[7] == 7 && buf[8] == 4;
    if (!ok) { std::printf("00411f10_TEST FAIL\n"); return 1; }
    std::printf("00411f10_TEST PASS\n");
    return 0;
}