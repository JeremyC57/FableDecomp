// Behaviour test for 0x00C0C270 CString::ConcatInPlace(int nSrcLen, const char* lpszSrcData)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Data { long nRefs, nDataLength, nAllocLength; };
struct Str { char* p; Data* GetData() const { return ((Data*)p) - 1; } };
static int g_copies, g_releases;
static void Make(Str& s, const char* text, long alloc, long refs)
{
    Data* d = (Data*)malloc(sizeof(Data) + alloc + 1);
    d->nRefs = refs; d->nDataLength = (long)strlen(text); d->nAllocLength = alloc;
    s.p = (char*)(d + 1); strcpy(s.p, text);
}
static void ConcatCopy(Str& s, int n1, const char* a, int n2, const char* b)   // stand-in for 0x00C0C220: fresh buffer
{
    ++g_copies;
    char tmp[256]; memcpy(tmp, a, n1); memcpy(tmp + n1, b, n2); tmp[n1 + n2] = 0;
    Make(s, tmp, n1 + n2, 1);
}
static void ConcatInPlace(Str& s, int nSrcLen, const char* src)   // the row's body
{
    if (nSrcLen == 0) return;
    if (s.GetData()->nRefs > 1 || s.GetData()->nDataLength + nSrcLen > s.GetData()->nAllocLength) {
        Data* old = s.GetData();
        ConcatCopy(s, old->nDataLength, s.p, nSrcLen, src);
        ++g_releases;
    } else {
        memcpy(s.p + s.GetData()->nDataLength, src, nSrcLen);
        s.GetData()->nDataLength += nSrcLen;
        s.p[s.GetData()->nDataLength] = 0;
    }
}
int main()
{
    Str s; Make(s, "abc", 10, 1);
    int ok = 1;
    ConcatInPlace(s, 0, "zzz");
    ok &= strcmp(s.p, "abc") == 0 && g_copies == 0;
    ConcatInPlace(s, 3, "def");                         // fits: in place
    ok &= strcmp(s.p, "abcdef") == 0 && s.GetData()->nDataLength == 6 && g_copies == 0;
    ConcatInPlace(s, 5, "ghijk");                       // 11 > 10: grow path
    ok &= strcmp(s.p, "abcdefghijk") == 0 && g_copies == 1 && g_releases == 1;
    Str t; Make(t, "x", 10, 2);                          // shared: copy even though it fits
    ConcatInPlace(t, 1, "y");
    ok &= strcmp(t.p, "xy") == 0 && g_copies == 2;
    printf(ok ? "00c0c270_TEST PASS\n" : "00c0c270_TEST FAIL\n");
    return ok ? 0 : 1;
}