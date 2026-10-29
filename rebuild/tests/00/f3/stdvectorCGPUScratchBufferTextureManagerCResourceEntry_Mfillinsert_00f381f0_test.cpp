// Behaviour test for 0x00F381F0 std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_fill_insert: insert n copies of
// x at pos; in place when capacity allows (the elems_after > n and elems_after <= n arms), else through _M_insert_overflow.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CResourceEntry { int ResourceType; int Format[4]; int Width, Height; int Usage; int Pool; int Pad[2]; };
static int g_overflows;
struct CVec {
    CResourceEntry* Start; CResourceEntry* Finish; CResourceEntry* End;
    int size() const { return (int)(Finish - Start); }
    void InsertOverflow(CResourceEntry* pos, const CResourceEntry& x, int n)
    {
        ++g_overflows;
        int oldSize = size(), len = oldSize + (oldSize > n ? oldSize : n);
        CResourceEntry* ns = (CResourceEntry*)malloc(len * sizeof(CResourceEntry)); CResourceEntry* nf = ns;
        for (CResourceEntry* p = Start; p != pos; ++p) *nf++ = *p;
        for (int i = 0; i < n; ++i) *nf++ = x;
        for (CResourceEntry* p = pos; p != Finish; ++p) *nf++ = *p;
        free(Start); Start = ns; Finish = nf; End = ns + len;
    }
    void FillInsert(CResourceEntry* pos, int n, const CResourceEntry& x)
    {
        if (n == 0) return;
        if (End - Finish >= n) {
            CResourceEntry xCopy = x;
            int elemsAfter = (int)(Finish - pos);
            CResourceEntry* oldFinish = Finish;
            if (elemsAfter > n) {
                memcpy(Finish, Finish - n, n * sizeof(CResourceEntry));
                Finish += n;
                for (CResourceEntry* d = oldFinish, *s = oldFinish - n; s != pos; ) *--d = *--s;
                for (CResourceEntry* p = pos; p != pos + n; ++p) *p = xCopy;
            } else {
                for (int i = 0; i < n - elemsAfter; ++i) Finish[i] = xCopy;
                Finish += n - elemsAfter;
                memcpy(Finish, pos, elemsAfter * sizeof(CResourceEntry));
                Finish += elemsAfter;
                for (CResourceEntry* p = pos; p != oldFinish; ++p) *p = xCopy;
            }
        } else
            InsertOverflow(pos, x, n);
    }
};
static int Widths(const CVec& v, const int* w, int n) { if (v.size() != n) return 0; for (int i = 0; i < n; ++i) if (v.Start[i].Width != w[i]) return 0; return 1; }

int main()
{
    CVec v; v.Start = (CResourceEntry*)calloc(10, sizeof(CResourceEntry)); v.Finish = v.Start + 4; v.End = v.Start + 10;
    for (int i = 0; i < 4; ++i) v.Start[i].Width = i + 1;
    CResourceEntry x; memset(&x, 0, sizeof(x)); x.Width = 9;
    v.FillInsert(v.Start + 1, 2, x);                       // elems_after 3 > 2
    static const int a[] = { 1, 9, 9, 2, 3, 4 };
    int ok = Widths(v, a, 6) && g_overflows == 0;
    v.FillInsert(v.Start + 5, 3, x);                       // elems_after 1 <= 3
    static const int b[] = { 1, 9, 9, 2, 3, 9, 9, 9, 4 };
    ok = ok && Widths(v, b, 9) && g_overflows == 0;
    v.FillInsert(v.Start, 2, x);                           // overflow
    ok = ok && v.size() == 11 && v.Start[0].Width == 9 && v.Start[2].Width == 1 && g_overflows == 1 && v.End - v.Start == 18;
    if (!ok) { printf("00f381f0_TEST FAIL\n"); return 1; }
    printf("00f381f0_TEST PASS\n");
    return 0;
}