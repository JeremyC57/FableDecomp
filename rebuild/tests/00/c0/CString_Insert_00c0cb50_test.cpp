// Behaviour test for 0x00C0CB50 CString::Insert(int nIndex, char ch) (libpk MFC-style string)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data { int nRefs, nDataLength, nAllocLength; char* data() { return (char*)(this + 1); } };
struct Str {
    char* p;
    Data* GetData() const { return ((Data*)p) - 1; }
    void AllocBuffer(int n)
    {
        Data* d = (Data*)malloc(sizeof(Data) + n + 1);
        d->nRefs = 1; d->nDataLength = n; d->nAllocLength = n; d->data()[n] = 0; p = d->data();
    }
    static void Release(Data* d) { if (--d->nRefs <= 0) free(d); }
    void CopyBeforeWrite()
    {
        if (GetData()->nRefs > 1) {
            Data* old = GetData(); --old->nRefs;
            AllocBuffer(old->nDataLength); memcpy(p, old->data(), old->nDataLength + 1);
        }
    }
    int Insert(int nIndex, char ch)                                 // the row's body
    {
        CopyBeforeWrite();
        if (nIndex < 0) nIndex = 0;
        int nNewLength = GetData()->nDataLength;
        if (nIndex > nNewLength) nIndex = nNewLength;
        nNewLength++;
        if (GetData()->nAllocLength < nNewLength) {
            Data* pOldData = GetData(); char* pstr = p;
            AllocBuffer(nNewLength);
            memcpy(p, pstr, pOldData->nDataLength + 1);
            Release(pOldData);
        }
        memmove(p + nIndex + 1, p + nIndex, nNewLength - nIndex);
        p[nIndex] = ch;
        GetData()->nDataLength = nNewLength;
        return nNewLength;
    }
};

int main()
{
    Str s; s.AllocBuffer(3); memcpy(s.p, "abc", 4);
    int ok = 1;
    ok &= s.Insert(1, 'X') == 4 && strcmp(s.p, "aXbc") == 0;
    ok &= s.Insert(-5, '<') == 5 && strcmp(s.p, "<aXbc") == 0;
    ok &= s.Insert(99, '>') == 6 && strcmp(s.p, "<aXbc>") == 0;
    Data* shared = s.GetData(); shared->nRefs = 2;                 // copy-before-write path
    ok &= s.Insert(0, '!') == 7 && strcmp(s.p, "!<aXbc>") == 0 && s.GetData() != shared && shared->nRefs == 1;
    printf(ok ? "00c0cb50_TEST PASS\n" : "00c0cb50_TEST FAIL\n");
    return ok ? 0 : 1;
}