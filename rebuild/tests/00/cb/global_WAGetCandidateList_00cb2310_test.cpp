// Behaviour test for 0x00CB2310 WA_GetCandidateList (DXUT ImeUi.cpp ANSI-IMM wrapper): the candidate list fetched in ANSI
// is rebuilt in wide characters (header copied, offsets rebased, strings widened) and its size returned.
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned int DWORD;                     // 32-bit on every host
struct CANDIDATELIST { DWORD dwSize, dwStyle, dwCount, dwSelection, dwPageStart, dwPageSize, dwOffset[1]; };

static int g_mallocs = 0, g_frees = 0;
static void* Malloc(unsigned int n) { ++g_mallocs; return malloc(n); }
static void Free(void* p) { ++g_frees; free(p); }

// ImmGetCandidateListA stand-in: two candidates "ab" and "xyz"
static DWORD ImmGetCandidateListA(void*, DWORD, CANDIDATELIST* list, DWORD len)
{
    const DWORD header = sizeof(CANDIDATELIST) + sizeof(DWORD);   // two offsets
    const DWORD size = header + 3 + 4;
    if (!list) return size;
    if (len < size) return 0;
    memset(list, 0, size);
    list->dwSize = size; list->dwCount = 2; list->dwSelection = 1;
    list->dwOffset[0] = header; list->dwOffset[1] = header + 3;
    memcpy((char*)list + header, "ab", 3); memcpy((char*)list + header + 3, "xyz", 4);
    return size;
}
static int MultiByteToWideChar(const char* src, unsigned short* dst, int cch)   // ASCII widening, counts the terminator
{
    int n = (int)strlen(src) + 1;
    if (dst) { if (cch < n) return 0; for (int i = 0; i < n; ++i) dst[i] = (unsigned char)src[i]; }
    return n;
}

static DWORD WA_GetCandidateList(void* himc, DWORD dwIndex, CANDIDATELIST** ppCandList)
{
    DWORD dwBufLen = ImmGetCandidateListA(himc, dwIndex, 0, 0);
    if (dwBufLen) {
        CANDIDATELIST* pCandList = (CANDIDATELIST*)Malloc(dwBufLen);
        if (pCandList) {
            dwBufLen = ImmGetCandidateListA(himc, dwIndex, pCandList, dwBufLen);
            if (dwBufLen) {
                int i;
                DWORD dwWideBufLen = 0;
                for (i = 0; i < (int)pCandList->dwCount; i++)
                    dwWideBufLen += MultiByteToWideChar((char*)pCandList + pCandList->dwOffset[i], 0, 0) * sizeof(unsigned short);
                dwWideBufLen += pCandList->dwOffset[0];
                *ppCandList = (CANDIDATELIST*)Malloc(dwWideBufLen);
                CANDIDATELIST* pCandListW = *ppCandList;
                memcpy(pCandListW, pCandList, pCandList->dwOffset[0]);
                unsigned short* pwz = (unsigned short*)((char*)pCandListW + pCandList->dwOffset[0]);
                for (i = 0; i < (int)pCandList->dwCount; i++) {
                    pCandListW->dwOffset[i] = (DWORD)((char*)pwz - (char*)pCandListW);
                    pwz += MultiByteToWideChar((char*)pCandList + pCandList->dwOffset[i], pwz, 256);
                }
                dwBufLen = dwWideBufLen;
            }
            Free(pCandList);
        }
    }
    return dwBufLen;
}

int main()
{
    CANDIDATELIST* w = 0;
    DWORD size = WA_GetCandidateList(0, 0, &w);
    const DWORD header = sizeof(CANDIDATELIST) + sizeof(DWORD);
    if (!w || size != header + 2 * (3 + 4) || g_mallocs != 2 || g_frees != 1) { std::printf("00cb2310_TEST FAIL\n"); return 1; }
    if (w->dwCount != 2 || w->dwSelection != 1 || w->dwOffset[0] != header || w->dwOffset[1] != header + 6) {
        std::printf("00cb2310_TEST FAIL\n"); return 1;
    }
    const unsigned short* s0 = (const unsigned short*)((char*)w + w->dwOffset[0]);
    const unsigned short* s1 = (const unsigned short*)((char*)w + w->dwOffset[1]);
    if (s0[0] != 'a' || s0[1] != 'b' || s0[2] != 0 || s1[0] != 'x' || s1[2] != 'z' || s1[3] != 0) { std::printf("00cb2310_TEST FAIL\n"); return 1; }
    free(w);
    std::printf("00cb2310_TEST PASS\n");
    return 0;
}