#include <cstdio>
// Behaviour model of the STLport red-black-tree in-order walk used by CTCHeroOpinionDeedLog (0x006C5680).
struct N { N* parent; N* left; N* right; int key; };
static N* Inc(N* x)
{
    if (x->right) { x = x->right; while (x->left) x = x->left; }
    else { N* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
int main()
{
    // header h: parent = root, left = leftmost, right = rightmost; tree 1 <- 2 -> 3
    N h, a, b, c;
    b.key = 2; a.key = 1; c.key = 3;
    h.parent = &b; h.left = &a; h.right = &c; h.key = -1;
    b.parent = &h; b.left = &a; b.right = &c;
    a.parent = &b; a.left = 0; a.right = 0;
    c.parent = &b; c.left = 0; c.right = 0;
    int seen[3], n = 0;
    for (N* it = h.left; it != &h; it = Inc(it)) { if (n < 3) seen[n] = it->key; ++n; }
    if (n != 3 || seen[0] != 1 || seen[1] != 2 || seen[2] != 3) { std::printf("006c5680_TEST FAIL\n"); return 1; }
    std::printf("006c5680_TEST PASS\n");
    return 0;
}