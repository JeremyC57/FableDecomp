// Behaviour test for 0x00A644C0 GFCheckTreeNodes(CMemoryAllocatorVariableSize::CMemoryArea*)
#include <cstdio>

class CMemoryAllocatorVariableSize {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
        CMemoryArea*      PNextArea;
        CMemoryArea*      PRightChild;
        CMemoryArea*      PPreviousArea;
        CMemoryArea*      PParentArea;
        CMemoryArea*      PLeftChild;
        CMemoryAreaChunk* PParentChunk;
        long              DataSize;
        unsigned char     Colour;
        unsigned char     ID;
    };
};
typedef CMemoryAllocatorVariableSize::CMemoryArea CMemoryArea;

bool GFCheckTreeNodes(CMemoryArea* node)
{
    if (node) {
        if (node->PLeftChild)
            GFCheckTreeNodes(node->PLeftChild);
        if (node->PRightChild)
            GFCheckTreeNodes(node->PRightChild);
    }
    return true;
}

int main()
{
    CMemoryArea n[5];
    for (int i = 0; i < 5; ++i) {
        n[i].PLeftChild = n[i].PRightChild = n[i].PParentArea = 0;
        n[i].DataSize = 16 * (i + 1);
    }
    n[0].PLeftChild = &n[1]; n[0].PRightChild = &n[2];      // a five-node tree walked depth first
    n[1].PLeftChild = &n[3]; n[2].PRightChild = &n[4];
    if (!GFCheckTreeNodes(&n[0]) || !GFCheckTreeNodes(&n[4]) || !GFCheckTreeNodes(0)) {
        std::printf("00a644c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a644c0_TEST PASS\n");
    return 0;
}