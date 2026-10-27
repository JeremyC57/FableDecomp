// Behaviour test for 0x00A76520 CNavNavigableLeafNode::HasNeighbours() const
#include <cstdio>
template <class T, int N> class CSmallVector { public: T* Start; signed char Size; };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode() { }
    virtual bool HasNeighbours() const = 0;
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    void* PRegion;
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;
    virtual bool HasNeighbours() const { return Neighbours.Size != 0; }
};
int main()
{
    CNavNavigableLeafNode node;
    CANavQuadTreeNode* base = &node;
    node.Neighbours.Start = 0;
    node.Neighbours.Size = 0;
    if (base->HasNeighbours()) { std::printf("00a76520_TEST FAIL\n"); return 1; }
    node.Neighbours.Size = 3;
    if (!base->HasNeighbours()) { std::printf("00a76520_TEST FAIL\n"); return 1; }
    node.Neighbours.Size = -128;
    if (!base->HasNeighbours()) { std::printf("00a76520_TEST FAIL\n"); return 1; }
    std::printf("00a76520_TEST PASS\n");
    return 0;
}