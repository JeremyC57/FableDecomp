#include <cstdio>
// Behaviour test for CNavNavigableLeafNode::RemoveNeighbours (0x00A76C10).
struct Leaf {
    void* PRegion; const void** Start; signed char Size;
    void RemoveNeighbours() { delete[] Start; Start = 0; Size = 0; PRegion = 0; }
};
int main()
{
    Leaf l; l.PRegion = (void*)&l; l.Start = new const void*[4]; l.Size = 3;
    l.RemoveNeighbours();
    bool ok = l.PRegion == 0 && l.Start == 0 && l.Size == 0;
    l.RemoveNeighbours();                    // empty list: delete[] of null is harmless
    ok = ok && l.Start == 0;
    std::printf(ok ? "00a76c10_TEST PASS\n" : "00a76c10_TEST FAIL\n");
    return ok ? 0 : 1;
}