#include <cstdio>
// Behaviour test for 00a64480: IsNodeBlack -- a null leaf or Colour == 1 is black.
struct Area { unsigned char Colour; };
static int IsNodeBlack(const Area* node) { if (node == 0 || node->Colour == 1) return 1; return 0; }
int main()
{
    Area red; red.Colour = 0; Area black; black.Colour = 1; Area other; other.Colour = 2;
    int ok = (IsNodeBlack(0) == 1 && IsNodeBlack(&black) == 1 && IsNodeBlack(&red) == 0 && IsNodeBlack(&other) == 0);
    if (!ok) { std::printf("00a64480_TEST FAIL\n"); return 1; }
    std::printf("00a64480_TEST PASS\n");
    return 0;
}