// Behaviour test for 00f3b8f0: std::__copy_backward over CArray<unsigned short> -- each element assigned through the vector operator=, returns the result end
#include <cstdio>
#include <vector>
typedef std::vector<unsigned short> Arr;
static int g_assigns = 0;
struct CArr : Arr { CArr& operator=(const CArr& o) { ++g_assigns; Arr::operator=(o); return *this; } };
static CArr* Fwd(CArr* first, CArr* last, CArr* result) { for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; } return result; }
static CArr* Bwd(CArr* first, CArr* last, CArr* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
int main()
{
    CArr a[5];
    for (int i = 0; i < 3; ++i) a[i].assign(i + 1, (unsigned short)(i + 10));
    int ok;
    CArr* r = Bwd(a, a + 3, a + 5);
    ok = r == a + 2 && g_assigns == 3 && a[2].size() == 1 && a[2][0] == 10 && a[3].size() == 2 && a[3][0] == 11 && a[4].size() == 3 && a[4][2] == 12;
    ok = ok && Bwd(a, a, a + 1) == a + 1 && g_assigns == 3;
    std::printf(ok ? "00f3b8f0_TEST PASS\n" : "00f3b8f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
