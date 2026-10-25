// Behaviour test for 0x00CB2670 ImeUi::GetCandidateList: query the size, allocate through ImeUiCallback_Malloc and
// fill the list; 0 (nothing allocated) when the IME has no list.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct tagCANDIDATELIST { unsigned int dwSize, dwStyle, dwCount, dwSelection, dwPageStart, dwPageSize, dwOffset[1]; };

static unsigned int g_listSize = 0;
static int g_calls = 0, g_mallocs = 0;
static unsigned int ImmGetCandidateList_(void* himc, unsigned int dwIndex, tagCANDIDATELIST* p, unsigned int dwBufLen)
{
    ++g_calls;
    if (himc == 0 || dwIndex != 0) return 0;
    if (p == 0) return g_listSize;
    if (dwBufLen < g_listSize) return 0;
    std::memset(p, 0, g_listSize);
    p->dwSize = g_listSize;
    p->dwCount = 3;
    return g_listSize;
}
static unsigned int (*_ImmGetCandidateList)(void*, unsigned int, tagCANDIDATELIST*, unsigned int) = ImmGetCandidateList_;
static void* Malloc_(unsigned int bytes) { ++g_mallocs; return std::malloc(bytes); }
static void* (*ImeUiCallback_Malloc)(unsigned int) = Malloc_;

static unsigned int GetCandidateList(void* himc, unsigned int dwIndex, tagCANDIDATELIST** ppCandList)
{
    unsigned int dwBufLen = _ImmGetCandidateList(himc, dwIndex, 0, 0);
    if (dwBufLen) {
        *ppCandList = (tagCANDIDATELIST*)ImeUiCallback_Malloc(dwBufLen);
        dwBufLen = _ImmGetCandidateList(himc, dwIndex, *ppCandList, dwBufLen);
    }
    return dwBufLen;
}

int main()
{
    int himc = 1;
    tagCANDIDATELIST* p = 0;
    g_listSize = 0;
    if (GetCandidateList(&himc, 0, &p) != 0 || p != 0 || g_calls != 1 || g_mallocs != 0) { std::printf("00cb2670_TEST FAIL\n"); return 1; }
    g_listSize = 64;
    if (GetCandidateList(&himc, 0, &p) != 64 || p == 0 || p->dwSize != 64 || p->dwCount != 3 || g_calls != 3 || g_mallocs != 1) {
        std::printf("00cb2670_TEST FAIL\n");
        return 1;
    }
    std::free(p);
    std::printf("00cb2670_TEST PASS\n");
    return 0;
}