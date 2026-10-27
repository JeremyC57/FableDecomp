#include <cstdio>
// CMediaPosition::GetTypeInfoCount: E_POINTER for null, else *pctinfo = 1, S_OK.
static int GetTypeInfoCount(unsigned* p) { if (p == 0) return (int)0x80004003; *p = 1; return 0; }
int main()
{
    unsigned n = 7;
    if (GetTypeInfoCount(0) != (int)0x80004003 || GetTypeInfoCount(&n) != 0 || n != 1) { std::printf("00cad290_TEST FAIL\n"); return 1; }
    std::printf("00cad290_TEST PASS\n");
    return 0;
}