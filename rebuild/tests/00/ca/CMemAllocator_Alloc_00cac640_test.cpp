// Behaviour test for 0x00CAC640 CMemAllocator::Alloc (strmbase amfilter.cpp, DX9 SDK): a copy of the logic over a small model.
#include <cstdio>
#include <cstdlib>

typedef int HRESULT;   // int: x64 g++ long is 64-bit
#define S_FALSE 1
#define E_OUTOFMEMORY ((HRESULT)0x8007000E)
#define VFW_E_SIZENOTSET ((HRESULT)0x80040212)

static int g_locks = 0, g_unlocks = 0, g_reallyFree = 0, g_failAlloc = 0;
static unsigned long g_lastSize = 0;
static void* FakeVirtualAlloc(unsigned long size) { g_lastSize = size; return g_failAlloc ? 0 : std::malloc(size); }

struct CMediaSample {
    unsigned long m_dwFlags, m_dwTypeSpecificFlags;
    unsigned char* m_pBuffer;
    long m_lActual, m_cbBuffer;
    void* m_pAllocator;
    CMediaSample* m_pNext;
    void* m_pMediaType;
    unsigned long m_dwStreamId;
    long m_cRef;
    CMediaSample(void* pAllocator, unsigned char* pBuffer, long length)
        : m_dwFlags(0), m_dwTypeSpecificFlags(0), m_pBuffer(pBuffer), m_lActual(length), m_cbBuffer(length),
          m_pAllocator(pAllocator), m_pNext(0), m_pMediaType(0), m_dwStreamId(0), m_cRef(0) {}
};

struct CMemAllocator {
    CMediaSample* m_List; int m_nOnList;
    long m_lCount, m_lAllocated, m_lSize, m_lAlignment, m_lPrefix;
    int m_bChanged;
    unsigned char* m_pBuffer;
    void ReallyFree()
    {
        g_reallyFree++;
        while (m_List) { CMediaSample* s = m_List; m_List = s->m_pNext; m_nOnList--; delete s; }
        m_lAllocated = 0;
        std::free(m_pBuffer); m_pBuffer = 0;
    }
    HRESULT BaseAlloc()
    {
        if (m_lCount <= 0 || m_lSize <= 0 || m_lAlignment <= 0) return VFW_E_SIZENOTSET;
        if (m_bChanged == 0) return S_FALSE;
        return 0;
    }
    HRESULT Alloc()
    {
        g_locks++;
        HRESULT hr = BaseAlloc();
        if (hr < 0) { g_unlocks++; return hr; }
        if (hr == S_FALSE) { g_unlocks++; return 0; }
        if (m_pBuffer) ReallyFree();
        long lAlignedSize = m_lSize + m_lPrefix;
        if (m_lAlignment > 1) {
            long lRemainder = lAlignedSize % m_lAlignment;
            if (lRemainder != 0) lAlignedSize += (m_lAlignment - lRemainder);
        }
        m_pBuffer = (unsigned char*)FakeVirtualAlloc(m_lCount * lAlignedSize);
        if (m_pBuffer == 0) { g_unlocks++; return E_OUTOFMEMORY; }
        unsigned char* pNext = m_pBuffer;
        for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {
            CMediaSample* pSample = new CMediaSample(this, pNext + m_lPrefix, m_lSize);
            pSample->m_pNext = m_List; m_List = pSample; m_nOnList++;
        }
        m_bChanged = 0;
        g_unlocks++;
        return 0;
    }
};

int main()
{
    CMemAllocator a;
    a.m_List = 0; a.m_nOnList = 0; a.m_lCount = 0; a.m_lAllocated = 0; a.m_lSize = 0; a.m_lAlignment = 0;
    a.m_lPrefix = 0; a.m_bChanged = 1; a.m_pBuffer = 0;
    int fail = 0;
    if (a.Alloc() != VFW_E_SIZENOTSET) fail |= 1;
    a.m_lCount = 3; a.m_lSize = 10; a.m_lAlignment = 8; a.m_lPrefix = 4;
    if (a.Alloc() != 0) fail |= 2;
    if (g_lastSize != 3 * 16 || a.m_lAllocated != 3 || a.m_nOnList != 3 || a.m_bChanged != 0) fail |= 4;
    /* the head is the last sample carved: buffer + 2*16 + prefix */
    if (a.m_List->m_pBuffer != a.m_pBuffer + 32 + 4 || a.m_List->m_cbBuffer != 10 || a.m_List->m_lActual != 10 || a.m_List->m_pAllocator != &a) fail |= 8;
    if (a.Alloc() != 0 || g_reallyFree != 0) fail |= 16;           /* unchanged: S_FALSE -> NOERROR, nothing freed */
    a.m_bChanged = 1; a.m_lCount = 2;
    if (a.Alloc() != 0 || g_reallyFree != 1 || a.m_nOnList != 2 || g_lastSize != 2 * 16) fail |= 32;
    a.m_bChanged = 1; g_failAlloc = 1;
    if (a.Alloc() != E_OUTOFMEMORY || g_reallyFree != 2) fail |= 64;
    if (g_locks != g_unlocks || g_locks != 5) fail |= 128;
    if (fail) { std::printf("00cac640_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cac640_TEST PASS\n");
    return 0;
}