// Behaviour test for the library CString::GetBuffer (0x00C0C4B0, catalogued CCharString::SetLength): a shared or
// too-small buffer is replaced by a private copy (at least the old length) keeping the text and length; a
// private, large-enough buffer is returned as is.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_asserts = 0;
#define LIB_ASSERT(cond) if (!(cond)) { ++g_asserts; }
struct CStringData {
    long nRefs, nDataLength, nAllocLength;
    char* data() { return (char*)(this + 1); }
};
static int g_released = 0;
class CString {
public:
    char* m_pchData;
    CStringData* GetData() const { LIB_ASSERT(m_pchData != NULL) return ((CStringData*)m_pchData) - 1; }
    void AllocBuffer(int nLen)
    {
        CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + nLen + 1);
        d->nRefs = 1; d->nDataLength = nLen; d->nAllocLength = nLen; d->data()[nLen] = 0;
        m_pchData = d->data();
    }
    static void Release(CStringData* p) { ++g_released; if (--p->nRefs <= 0) std::free(p); }
    char* GetBuffer(int nMinBufLength);
};

char* CString::GetBuffer(int nMinBufLength)
{
    LIB_ASSERT(nMinBufLength >= 0)
    if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength) {
        CStringData* pOldData = GetData();
        int nOldLen = GetData()->nDataLength;
        if (nMinBufLength < nOldLen)
            nMinBufLength = nOldLen;
        AllocBuffer(nMinBufLength);
        std::memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(char));
        GetData()->nDataLength = nOldLen;
        CString::Release(pOldData);
    }
    LIB_ASSERT(GetData()->nRefs <= 1)
    LIB_ASSERT(m_pchData != NULL)
    return m_pchData;
}

int main()
{
    CString a; a.AllocBuffer(5); std::strcpy(a.m_pchData, "hello");
    char* p = a.GetBuffer(3);                              // private and big enough: unchanged
    bool ok = p == a.m_pchData && a.GetData()->nAllocLength == 5 && g_released == 0;
    CStringData* old = a.GetData();
    p = a.GetBuffer(10);                                   // grows, keeps text and length
    ok = ok && p != old->data() && std::strcmp(p, "hello") == 0 && a.GetData()->nDataLength == 5
        && a.GetData()->nAllocLength == 10 && g_released == 1;
    CString b; b.m_pchData = a.m_pchData; a.GetData()->nRefs = 2;   // shared: b gets a private copy
    p = b.GetBuffer(2);
    ok = ok && p != a.m_pchData && std::strcmp(p, "hello") == 0 && b.GetData()->nAllocLength == 5
        && a.GetData()->nRefs == 1 && g_asserts == 0;
    if (!ok) { std::printf("00c0c4b0_TEST FAIL\n"); return 1; }
    std::printf("00c0c4b0_TEST PASS\n");
    return 0;
}