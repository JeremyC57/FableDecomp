#include <cstdio>
typedef unsigned short wch;
struct Data { wch buf[32]; int len; int NoRefs; };
static int g_unshare = 0;
struct WStr {
    Data* p;
    void MakeOwnCopyOfData() { ++g_unshare; }
    WStr& operator+=(const WStr& o)
    {
        if (p) {
            MakeOwnCopyOfData();
            static wch empty[1] = { 0 };
            const wch* t = o.p == 0 ? empty : o.p->buf;
            int n = 0; while (t[n]) ++n;
            for (int i = 0; i < n; ++i) p->buf[p->len++] = t[i];
            p->buf[p->len] = 0;
        } else if (&o != this && o.p) { p = o.p; ++p->NoRefs; }
        return *this;
    }
};
int main()
{
    Data a; a.buf[0] = 'x'; a.buf[1] = 0; a.len = 1; a.NoRefs = 1;
    Data b; b.buf[0] = 'y'; b.buf[1] = 'z'; b.buf[2] = 0; b.len = 2; b.NoRefs = 1;
    WStr s1; s1.p = &a; WStr s2; s2.p = &b; WStr s3; s3.p = 0; WStr s4; s4.p = 0;
    s1 += s2;
    if (a.len != 3 || a.buf[2] != 'z' || g_unshare != 1) { std::printf("0099b8d0_TEST FAIL\n"); return 1; }
    s3 += s2; if (s3.p != &b || b.NoRefs != 2) { std::printf("0099b8d0_TEST FAIL\n"); return 1; }
    s4 += s4; WStr s5; s5.p = 0; s4 += s5; if (s4.p != 0) { std::printf("0099b8d0_TEST FAIL\n"); return 1; }
    s1 += s5; if (a.len != 3 || g_unshare != 2) { std::printf("0099b8d0_TEST FAIL\n"); return 1; }
    std::printf("0099b8d0_TEST PASS\n");
    return 0;
}
