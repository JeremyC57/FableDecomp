// Behaviour test for 0x00C0D7C0 CString::TrimRight(char) (label CCharString::RemoveAllOccurrences).
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CStringData { long nRefs; long nDataLength; long nAllocLength; };
static CStringData g_nilData = { -1, 0, 0 };
static CStringData* _afxDataNil = &g_nilData;
static int g_freed = 0;
struct CString {
    char* m_pchData;
    CStringData* GetData() const { return ((CStringData*)m_pchData) - 1; }
    void CopyBeforeWrite() { }
    static void FreeData(CStringData* data) { ++g_freed; std::free(data); }
    static CString Make(const char* text, long alloc)
    {
        CString s; CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + alloc + 1);
        d->nRefs = 1; d->nDataLength = (long)std::strlen(text); d->nAllocLength = alloc;
        s.m_pchData = (char*)(d + 1); std::strcpy(s.m_pchData, text); return s;
    }
};
void TrimRight(CString& s, char chTarget)
{
    s.CopyBeforeWrite();
    char* lpsz = s.m_pchData;
    char* lpszLast = 0;
    while (*lpsz != '\0') {
        if (*lpsz == chTarget) { if (lpszLast == 0) lpszLast = lpsz; }
        else lpszLast = 0;
        lpsz = lpsz + 1;                                 // _mbsinc on single-byte text
    }
    if (lpszLast != 0) { *lpszLast = '\0'; s.GetData()->nDataLength = (long)(lpszLast - s.m_pchData); }
}
int main()
{
    CString s = CString::Make("a..b...", 16);
    TrimRight(s, '.');
    if (std::strcmp(s.m_pchData, "a..b") != 0 || s.GetData()->nDataLength != 4) { std::printf("00c0d7c0_TEST FAIL\n"); return 1; }
    TrimRight(s, '.');
    if (std::strcmp(s.m_pchData, "a..b") != 0) { std::printf("00c0d7c0_TEST FAIL\n"); return 1; }
    std::printf("00c0d7c0_TEST PASS\n");
    return 0;
}
