#include <cstdio>
#include <map>
// Each animated block's group bytes are replaced through the bone map.
struct Block { unsigned char GroupCount; unsigned char Groups[18]; };
int main() {
    std::map<unsigned int, unsigned int> remap; remap[3] = 7; remap[5] = 1;
    Block blocks[2] = { { 2, { 3, 5 } }, { 1, { 5 } } };
    for (unsigned int b = 0; b < 2; ++b) for (unsigned int g = 0; g < blocks[b].GroupCount; ++g) blocks[b].Groups[g] = (unsigned char)remap[blocks[b].Groups[g]];
    if (blocks[0].Groups[0] != 7 || blocks[0].Groups[1] != 1 || blocks[1].Groups[0] != 1) { std::printf("00a90f00_TEST FAIL\n"); return 1; }
    std::printf("00a90f00_TEST PASS\n"); return 0; }
