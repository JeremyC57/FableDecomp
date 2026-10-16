#include <cstdio>
struct Hdr { long nRefs; long nDataLength; long nAllocLength; };
static int g_asserts = 0, g_release = 0, g_alloc = 0;
static Hdr g_fresh = { 1, 0, 0 };
static char g_freshbuf[sizeof(Hdr) + 64];
struct Str {
    char* m_pchData;
    Hdr* GetData() const { if (!m_pchData) ++g_asserts; return ((Hdr*)m_pchData) - 1; }
    void Release() { ++g_release; }
    void AllocBuffer(int n) { ++g_alloc; Hdr* h = (Hdr*)g_freshbuf; h->nRefs = 1; h->nDataLength = 0; h->nAllocLength = n; m_pchData = (char*)(h + 1); }
    void AllocBeforeWrite(int n)
    {
        if (GetData()->nRefs > 1 || n > GetData()->nAllocLength) { Release(); AllocBuffer(n); }
        if (!(GetData()->nRefs <= 1)) ++g_asserts;
    }
};
int main()
{
    static char shared[sizeof(Hdr) + 16];
    Hdr* h = (Hdr*)shared; h->nRefs = 1; h->nDataLength = 0; h->nAllocLength = 16;
    Str s; s.m_pchData = (char*)(h + 1);
    s.AllocBeforeWrite(8);                    // fits, unshared: keeps the buffer
    bool ok1 = s.m_pchData == (char*)(h + 1) && g_alloc == 0;
    s.AllocBeforeWrite(32);                   // too small: reallocates
    bool ok2 = g_alloc == 1 && g_release == 1 && ((Hdr*)s.m_pchData - 1)->nAllocLength == 32;
    h->nRefs = 2; s.m_pchData = (char*)(h + 1);
    s.AllocBeforeWrite(4);                    // shared: reallocates
    bool ok3 = g_alloc == 2 && g_asserts == 0;
    (void)g_fresh;
    if (!ok1 || !ok2 || !ok3) { std::printf("00c0bee0_TEST FAIL\n"); return 1; }
    std::printf("00c0bee0_TEST PASS\n");
    return 0;
}