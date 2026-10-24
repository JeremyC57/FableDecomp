// Behaviour test for 0x00AD9030 CVertexGroupBlock::CVertexGroupBlock (empty vector + empty STLport map).
#include <cstdio>
#include <cstdlib>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; unsigned int Key; long Value; };
struct CVertexGroupBlock {
    unsigned int* GroupsStart; unsigned int* GroupsFinish; unsigned int* GroupsEnd;
    SNode* Header; unsigned int Count; char Compare;
    CVertexGroupBlock() : GroupsStart(0), GroupsFinish(0), GroupsEnd(0), Header(0)
    {
        Header = (SNode*)std::malloc(sizeof(SNode));
        Count = 0;
        Header->Color = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header;
    }
};

int main()
{
    CVertexGroupBlock b;
    if (b.GroupsStart || b.GroupsFinish || b.GroupsEnd) { std::printf("00ad9030_TEST FAIL\n"); return 1; }
    if (!b.Header || b.Count || b.Header->Color || b.Header->Parent || b.Header->Left != b.Header || b.Header->Right != b.Header) { std::printf("00ad9030_TEST FAIL\n"); return 1; }
    std::free(b.Header);
    std::printf("00ad9030_TEST PASS\n");
    return 0;
}