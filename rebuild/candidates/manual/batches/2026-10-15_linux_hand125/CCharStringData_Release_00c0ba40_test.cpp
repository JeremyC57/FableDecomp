// Behaviour test for 0x00C0BA40 CString::~CString (label CCharStringData::Release): nil block untouched, last reference frees.
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
void Destroy(CString& s)
{
    if (s.GetData() != _afxDataNil) {
        if (--s.GetData()->nRefs <= 0)
            CString::FreeData(s.GetData());
    }
}
int main()
{
    CString nil; nil.m_pchData = (char*)(_afxDataNil + 1);
    Destroy(nil);
    if (g_freed != 0 || g_nilData.nRefs != -1) { std::printf("00c0ba40_TEST FAIL\n"); return 1; }
    CString s = CString::Make("x", 4); s.GetData()->nRefs = 2;
    Destroy(s);
    if (g_freed != 0 || s.GetData()->nRefs != 1) { std::printf("00c0ba40_TEST FAIL\n"); return 1; }
    Destroy(s);
    if (g_freed != 1) { std::printf("00c0ba40_TEST FAIL\n"); return 1; }
    std::printf("00c0ba40_TEST PASS\n");
    return 0;
}
