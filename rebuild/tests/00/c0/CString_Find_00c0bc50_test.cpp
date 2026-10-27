// behaviour test for 00c0bc50 (standalone copy of the model)
#include <cstdio>
#include <cstring>

static int g_asserts = 0;
#define LIB_ASSERT(cond) if (!(cond)) { ++g_asserts; }
struct CStringData { int nRefs; int nDataLength; int nAllocLength; };
static char g_store[64];
static char* MakeString(const char* text) { CStringData* d = (CStringData*)g_store; d->nRefs = 1; d->nDataLength = (int)std::strlen(text); d->nAllocLength = d->nDataLength; char* p = (char*)(d + 1); std::strcpy(p, text); return p; }

static inline bool IsValidString(const char* p) { return p != NULL; }
class CString { public: char* m_pchData;
    CStringData* GetData() const { LIB_ASSERT(m_pchData != NULL) return ((CStringData*)m_pchData) - 1; }
    int Find(const char* lpszSub, int nStart) const { LIB_ASSERT(IsValidString(lpszSub)) int nLength = GetData()->nDataLength; if (nStart > nLength) return -1;
        const char* lpsz = std::strstr(m_pchData + nStart, lpszSub); return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData); } };

int main()
{
    CString s; s.m_pchData = MakeString("foobarfoo");
    if (!(s.Find("foo", 0) == 0 && s.Find("foo", 1) == 6 && s.Find("", 9) == 9 && s.Find("x", 0) == -1 && s.Find("foo", 10) == -1 && g_asserts == 0)) { std::printf("00c0bc50_TEST FAIL\n"); return 1; }
    std::printf("00c0bc50_TEST PASS\n");
    return 0;
}