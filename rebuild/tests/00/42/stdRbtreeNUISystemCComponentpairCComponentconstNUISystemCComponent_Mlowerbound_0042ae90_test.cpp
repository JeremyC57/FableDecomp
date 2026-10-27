// behaviour test for 0x0042AE90 _Rb_tree::_M_lower_bound (model on a three-node tree)
#include <cstdio>
struct N { N* parent; N* left; N* right; unsigned key; };
static N* LowerBound(N* header, unsigned k)
{
    N* y = header; N* x = header->parent;
    while (x) { if (!(x->key < k)) { y = x; x = x->left; } else x = x->right; }
    return y;
}
int main()
{
    N h = { 0, 0, 0, 0 }, a = { 0, 0, 0, 10 }, b = { 0, 0, 0, 20 }, c = { 0, 0, 0, 30 };
    h.parent = &b; b.left = &a; b.right = &c;
    int ok = LowerBound(&h, 15) == &b && LowerBound(&h, 20) == &b && LowerBound(&h, 5) == &a && LowerBound(&h, 31) == &h;
    if (!ok) { std::printf("0042ae90_TEST FAIL\n"); return 1; }
    std::printf("0042ae90_TEST PASS\n");
    return 0;
}