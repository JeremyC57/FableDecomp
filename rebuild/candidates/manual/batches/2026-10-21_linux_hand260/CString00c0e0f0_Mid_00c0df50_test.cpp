// Behaviour test for CString_00c0e0f0::Mid (0x00C0DF50): clamps, clipping, whole-string copy, slice copy, asserts
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_asserts = 0;
static int g_traces = 0;
static void LogSink(const char* text) { (void)text; ++g_asserts; }
static void (*g_pfnLogSink)(const char*) = LogSink;
struct CTrace { CTrace() { ++g_traces; } };
struct CStringData { long nRefs; long nDataLength; long nAllocLength; };
static long g_nil[4] = { -1, 0, 0, 0 };
struct CStr {
    char* m_pchData;
    CStr() : m_pchData((char*)(g_nil + 3)) {}
    explicit CStr(const char* s)
    {
        long n = (long)std::strlen(s);
        CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + n + 1);
        d->nRefs = 1; d->nDataLength = n; d->nAllocLength = n;
        m_pchData = (char*)(d + 1);
        std::memcpy(m_pchData, s, n + 1);
    }
    CStr(const CStr& o) : m_pchData(o.m_pchData) { if (GetData()->nRefs >= 0) ++GetData()->nRefs; }
    ~CStr() { if (GetData()->nRefs >= 0 && --GetData()->nRefs == 0) std::free(GetData()); }
    CStringData* GetData() const
    {
        if (m_pchData == 0) {
            if (g_pfnLogSink) g_pfnLogSink("m_pchData != NULL\n");
            CTrace();
        }
        return ((CStringData*)m_pchData) - 1;
    }
    void AllocCopy(CStr& dest, long copyLen, long copyIndex, long extraLen) const
    {
        long n = copyLen + extraLen;
        if (n == 0) return;
        CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + n + 1);
        d->nRefs = 1; d->nDataLength = n; d->nAllocLength = n;
        dest.m_pchData = (char*)(d + 1);
        std::memcpy(dest.m_pchData, m_pchData + copyIndex, copyLen);
        dest.m_pchData[n] = 0;
    }
    CStr Mid(long nFirst, long nCount) const
    {
        if (nFirst < 0) nFirst = 0;
        if (nCount < 0) nCount = 0;
        if (nFirst + nCount > GetData()->nDataLength) nCount = GetData()->nDataLength - nFirst;
        if (nFirst > GetData()->nDataLength) nCount = 0;
        if (!(nFirst >= 0)) { if (g_pfnLogSink) g_pfnLogSink("nFirst >= 0\n"); CTrace(); }
        if (!(nFirst + nCount <= GetData()->nDataLength)) { if (g_pfnLogSink) g_pfnLogSink("x\n"); CTrace(); }
        if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
            return *this;
        {
            CStr dest;
            AllocCopy(dest, nCount, nFirst, 0);
            return dest;
        }
    }
};
int main()
{
    int bad = 0;
    CStr s("Hello world");
    { CStr r = s.Mid(0, 100); if (r.m_pchData != s.m_pchData || s.GetData()->nRefs != 2) bad |= 1; }
    { CStr r = s.Mid(6, 5); if (std::strcmp(r.m_pchData, "world") != 0) bad |= 2; }
    { CStr r = s.Mid(-3, 5); if (std::strcmp(r.m_pchData, "Hello") != 0) bad |= 4; }
    { CStr r = s.Mid(4, -2); if (r.GetData()->nDataLength != 0) bad |= 8; }
    { CStr r = s.Mid(20, 3); if (r.GetData()->nDataLength != 0) bad |= 16; }
    { CStr r = s.Mid(8, 50); if (std::strcmp(r.m_pchData, "rld") != 0) bad |= 32; }
    if (g_asserts != 1 || g_traces != 1) bad |= 64;   // Mid(20, 3) trips the second ASSERT
    if (bad) { std::printf("00c0df50_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c0df50_TEST PASS\n");
    return 0;
}
