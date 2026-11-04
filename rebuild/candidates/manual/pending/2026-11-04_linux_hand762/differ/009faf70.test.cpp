// Behaviour test for CTexture::GenerateMipmapsWithGPU (0x009FAF70, parked DIFFER): a model of the row -- levels 1..n-1 are each rendered from the level above, then the previous target is restored.
#include <stdio.h>
#include <vector>
static void Gen(std::vector<int>& levels, int& target)
{ int saved = target; for (size_t i = 1; i < levels.size(); ++i) { target = (int)i; levels[i] = levels[i - 1] / 2; } target = saved; }
int main() { std::vector<int> l(4, 0); l[0] = 64; int t = 9; Gen(l, t); bool ok = l[1] == 32 && l[3] == 8 && t == 9;
  printf(ok ? "009faf70_TEST PASS\n" : "009faf70_TEST FAIL\n");
  return ok ? 0 : 1;
}
