// behaviour test for 0x004B9D50: upper_bound returns the first key greater than k
#include <cstdio>
#include <cstring>
struct N { const char* k; N* l; N* r; };
static N* Upper(N* root, N* header, const char* k) { N* y = header; N* x = root; while (x) { if (std::strcmp(k, x->k) < 0) { y = x; x = x->l; } else x = x->r; } return y; }
int main()
{
    N a = { "a", 0, 0 }, c = { "c", 0, 0 }, b = { "b", &a, &c }, h = { "", 0, 0 };
    int ok = Upper(&b, &h, "a") == &b && Upper(&b, &h, "b") == &c && Upper(&b, &h, "c") == &h && Upper(&b, &h, "") == &a;
    if (!ok) { std::printf("004b9d50_TEST FAIL\n"); return 1; }
    std::printf("004b9d50_TEST PASS\n");
    return 0;
}