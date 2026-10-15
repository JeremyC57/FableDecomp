// Behaviour test for 0x00C0D970 CString::TrimLeft(char) (label CCharString::RemoveAllOccurrencesAndMove).
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
void TrimLeft(CString& s, char chTarget)
{
    s.CopyBeforeWrite();
    const char* lpsz = s.m_pchData;
    while (chTarget == *lpsz) lpsz = lpsz + 1;           // _mbsinc on single-byte text
    if (lpsz != s.m_pchData) {
        int nDataLength = s.GetData()->nDataLength - (int)(lpsz - s.m_pchData);
        std::memmove(s.m_pchData, lpsz, (nDataLength + 1) * sizeof(char));
        s.GetData()->nDataLength = nDataLength;
    }
}
int main()
{
    CString s = CString::Make("   xy ", 16);
    TrimLeft(s, ' ');
    if (std::strcmp(s.m_pchData, "xy ") != 0 || s.GetData()->nDataLength != 3) { std::printf("00c0d970_TEST FAIL\n"); return 1; }
    TrimLeft(s, ' ');
    if (std::strcmp(s.m_pchData, "xy ") != 0) { std::printf("00c0d970_TEST FAIL\n"); return 1; }
    std::printf("00c0d970_TEST PASS\n");
    return 0;
}
