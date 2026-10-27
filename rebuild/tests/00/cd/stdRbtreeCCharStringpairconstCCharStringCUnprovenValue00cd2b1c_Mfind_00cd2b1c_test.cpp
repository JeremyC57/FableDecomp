// behaviour test for 0x00CD2B1C _Rb_tree<CCharString,...>::_M_find (model over string keys)
#include <cstdio>
#include <cstring>
struct N { N* parent; N* left; N* right; const char* key; };
static bool Less(const char* a, const char* b) { return std::strcmp(a, b) < 0; }
static N* Find(N* header, const char* k)
{
    N* y = header; N* x = header->parent;
    while (x) { if (!Less(x->key, k)) { y = x; x = x->left; } else x = x->right; }
    if (y == header || Less(k, y->key)) y = header;
    return y;
}
int main()
{
    N h = { 0, 0, 0, "" }, a = { 0, 0, 0, "alpha" }, b = { 0, 0, 0, "beta" }, c = { 0, 0, 0, "gamma" };
    h.parent = &b; b.left = &a; b.right = &c;
    int ok = Find(&h, "beta") == &b && Find(&h, "gamma") == &c && Find(&h, "delta") == &h && Find(&h, "zeta") == &h;
    if (!ok) { std::printf("00cd2b1c_TEST FAIL\n"); return 1; }
    std::printf("00cd2b1c_TEST PASS\n");
    return 0;
}