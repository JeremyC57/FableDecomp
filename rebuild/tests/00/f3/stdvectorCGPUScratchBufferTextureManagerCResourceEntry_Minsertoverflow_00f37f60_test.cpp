// Behaviour test for 0x00F37F60 std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_insert_overflow: a POD-like
// 44-byte element; grow to max(2 * size, size + n), copy the prefix, n copies of x (n == 1 a single memberwise copy), the
// suffix unless atend, free the old block and adopt the new one.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CResourceEntry { int ResourceType; int Format[4]; int Width, Height; int Usage; int Pool; int Pad[2]; };
struct CVec {
    CResourceEntry* Start; CResourceEntry* Finish; CResourceEntry* End;
    int size() const { return (int)(Finish - Start); }
    void InsertOverflow(CResourceEntry* pos, const CResourceEntry& x, int n, bool atend)
    {
        int oldSize = size();
        int len = oldSize + (oldSize > n ? oldSize : n);
        CResourceEntry* newStart = (CResourceEntry*)malloc(len * sizeof(CResourceEntry));
        CResourceEntry* newFinish = newStart;
        for (CResourceEntry* p = Start; p != pos; ++p) *newFinish++ = *p;
        if (n == 1) *newFinish++ = x;
        else for (int i = 0; i < n; ++i) *newFinish++ = x;
        if (!atend) for (CResourceEntry* p = pos; p != Finish; ++p) *newFinish++ = *p;
        free(Start);
        Start = newStart; Finish = newFinish; End = newStart + len;
    }
};

int main()
{
    int ok = sizeof(CResourceEntry) == 44;
    CVec v; v.Start = (CResourceEntry*)malloc(2 * sizeof(CResourceEntry)); v.Finish = v.Start + 2; v.End = v.Finish;
    memset(v.Start, 0, 2 * sizeof(CResourceEntry)); v.Start[0].Width = 1; v.Start[1].Width = 2;
    CResourceEntry x; memset(&x, 0, sizeof(x)); x.Width = 7; x.Height = 8;
    v.InsertOverflow(v.Start + 1, x, 1, false);
    ok = ok && v.size() == 3 && v.End - v.Start == 4 && v.Start[0].Width == 1 && v.Start[1].Width == 7 && v.Start[1].Height == 8
        && v.Start[2].Width == 2;
    v.InsertOverflow(v.Finish, x, 5, true);
    ok = ok && v.size() == 8 && v.End - v.Start == 8 && v.Start[7].Width == 7;
    if (!ok) { printf("00f37f60_TEST FAIL\n"); return 1; }
    printf("00f37f60_TEST PASS\n");
    return 0;
}