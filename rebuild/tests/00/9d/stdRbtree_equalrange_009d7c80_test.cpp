// behaviour test for 0x009D7C80: equal_range brackets the matching key (empty range for a missing key)
#include <cstdio>
struct N { unsigned long k; N* l; N* r; };
static N* Lower(N* root, N* h, unsigned long k) { N* y = h; N* x = root; while (x) { if (!(x->k < k)) { y = x; x = x->l; } else x = x->r; } return y; }
static N* Upper(N* root, N* h, unsigned long k) { N* y = h; N* x = root; while (x) { if (k < x->k) { y = x; x = x->l; } else x = x->r; } return y; }
int main()
{
    N a = { 10, 0, 0 }, c = { 30, 0, 0 }, b = { 20, &a, &c }, h = { 0, 0, 0 };
    int ok = Lower(&b, &h, 20) == &b && Upper(&b, &h, 20) == &c && Lower(&b, &h, 25) == &c && Upper(&b, &h, 25) == &c && Upper(&b, &h, 30) == &h;
    if (!ok) { std::printf("009d7c80_TEST FAIL\n"); return 1; }
    std::printf("009d7c80_TEST PASS\n");
    return 0;
}