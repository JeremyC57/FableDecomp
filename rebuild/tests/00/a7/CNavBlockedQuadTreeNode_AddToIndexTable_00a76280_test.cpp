// Behaviour test for 0x00A76280 CNavBlockedQuadTreeNode::AddToIndexTable(long&, map&) const -- a blocked node takes an index slot but adds no entry
#include <cstdio>
class CIndexTable { public: int Entries; };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode() { }
    virtual void AddToIndexTable(long& index, CIndexTable& table) const = 0;
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
public:
    virtual void AddToIndexTable(long& index, CIndexTable&) const { ++index; }
};
int main()
{
    CNavBlockedQuadTreeNode node;
    CANavQuadTreeNode* base = &node;
    CIndexTable table; table.Entries = 0;
    long index = 7;
    base->AddToIndexTable(index, table);
    base->AddToIndexTable(index, table);
    if (index != 9 || table.Entries != 0) { std::printf("00a76280_TEST FAIL\n"); return 1; }
    std::printf("00a76280_TEST PASS\n");
    return 0;
}