// Behaviour test for 0x00C0C630 CString::ReleaseBuffer(int) (label CCharString::SetLengthAuto).
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
void ReleaseBuffer(CString& s, int nNewLength)
{
    s.CopyBeforeWrite();
    if (nNewLength == -1) nNewLength = (int)std::strlen(s.m_pchData);
    s.GetData()->nDataLength = nNewLength;
    s.m_pchData[nNewLength] = '\0';
}
int main()
{
    CString s = CString::Make("hello world", 32);
    ReleaseBuffer(s, 5);
    if (s.GetData()->nDataLength != 5 || std::strcmp(s.m_pchData, "hello") != 0) { std::printf("00c0c630_TEST FAIL\n"); return 1; }
    s.m_pchData[2] = 0;
    ReleaseBuffer(s, -1);
    if (s.GetData()->nDataLength != 2) { std::printf("00c0c630_TEST FAIL\n"); return 1; }
    std::printf("00c0c630_TEST PASS\n");
    return 0;
}