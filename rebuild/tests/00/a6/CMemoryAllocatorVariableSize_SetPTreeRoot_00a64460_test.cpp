// Behaviour test for 00a64460 CMemoryAllocatorVariableSize::SetPTreeRoot: the free-area tree root accessor pair.
#include <cstdio>
struct CMemoryArea { int id; };
struct CMemoryAllocatorVariableSize {
    unsigned char _pad[0x20];
    unsigned long ExtraMemUsedByOS;
    CMemoryArea*  PTreeRoot;
    void SetPTreeRoot(CMemoryArea* root) { PTreeRoot = root; }
    CMemoryArea* GetPTreeRoot() { return PTreeRoot; }
};
int main()
{
    CMemoryArea a = { 1 }, b = { 2 };
    CMemoryAllocatorVariableSize v; v.ExtraMemUsedByOS = 77; v.PTreeRoot = 0;
    v.SetPTreeRoot(&a);
    if (v.GetPTreeRoot() != &a || v.ExtraMemUsedByOS != 77) { std::printf("00a64460_TEST FAIL\n"); return 1; }
    v.SetPTreeRoot(&b);
    if (v.GetPTreeRoot()->id != 2) { std::printf("00a64460_TEST FAIL\n"); return 1; }
    v.SetPTreeRoot(0);
    if (v.GetPTreeRoot() != 0) { std::printf("00a64460_TEST FAIL\n"); return 1; }
    std::printf("00a64460_TEST PASS\n");
    return 0;
}