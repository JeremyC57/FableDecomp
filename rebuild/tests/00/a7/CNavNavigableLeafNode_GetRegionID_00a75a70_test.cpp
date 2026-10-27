// Behaviour test for 0x00A75A70 CNavNavigableLeafNode::GetRegionID() const
#include <cstdio>
class CNavRegion { public: long RegionID; long GetRegionID() const { return RegionID; } };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode() { }
    virtual long GetRegionID() const = 0;
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion* PRegion;
    virtual long GetRegionID() const
    {
        if (PRegion == 0)
            return 0;
        return PRegion->GetRegionID();
    }
};
int main()
{
    CNavRegion region; region.RegionID = 42;
    CNavNavigableLeafNode node;
    CANavQuadTreeNode* base = &node;
    node.PRegion = 0;
    if (base->GetRegionID() != 0) { std::printf("00a75a70_TEST FAIL\n"); return 1; }
    node.PRegion = &region;
    if (base->GetRegionID() != 42) { std::printf("00a75a70_TEST FAIL\n"); return 1; }
    std::printf("00a75a70_TEST PASS\n");
    return 0;
}