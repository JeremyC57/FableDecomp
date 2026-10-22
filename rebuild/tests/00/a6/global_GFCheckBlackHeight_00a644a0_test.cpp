#include <cstdio>
// Behaviour test for 00a644a0: GFCheckBlackHeight adds the black nodes down the left-child chain to the count.
struct Area { Area* PLeftChild; unsigned char Colour; };
static long GFCheckBlackHeight(Area* node, long count);
static long Below(Area* left, long count) { if (left != 0) return GFCheckBlackHeight(left, count); return count; }
static long GFCheckBlackHeight(Area* node, long count) { if (node->Colour == 1) ++count; return Below(node->PLeftChild, count); }
int main()
{
    Area c; c.PLeftChild = 0; c.Colour = 1;
    Area b; b.PLeftChild = &c; b.Colour = 0;
    Area a; a.PLeftChild = &b; a.Colour = 1;
    int ok = (GFCheckBlackHeight(&a, 0) == 2 && GFCheckBlackHeight(&b, 5) == 6 && GFCheckBlackHeight(&c, 0) == 1);
    if (!ok) { std::printf("00a644a0_TEST FAIL\n"); return 1; }
    std::printf("00a644a0_TEST PASS\n");
    return 0;
}