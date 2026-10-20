// Behaviour test for 0x00A77D60 CNavQuadTreeNode::SaveToFile: base save, then the four child indices (TL, TR, BL, BR)
// looked up in the node map and written as 4-byte longs, then each child saves with flag=false.
#include <cstdio>
#include <map>

struct C2DVector { float X, Y; };
static long g_written[16]; static int g_nw; static int g_baseSaves; static int g_childFlags;
struct CAFile { void Write(const void* d, unsigned long size, bool) { if (size == 4) g_written[g_nw++] = *(const long*)d; } };
struct CANavQuadTreeNode;
typedef std::map<const CANavQuadTreeNode*, long> CNodeIndexMap;
struct CANavQuadTreeNode {
    long id;
    virtual ~CANavQuadTreeNode() {}
    virtual void SaveToFile(CAFile& file, const CNodeIndexMap& m, bool flag, const C2DVector& pos) const
    {
        ++g_baseSaves; if (flag) ++g_childFlags; long v = 1000 + id; file.Write(&v, 4, false);
    }
};
struct CNavQuadTreeNode : CANavQuadTreeNode {
    const CANavQuadTreeNode* TLNode; const CANavQuadTreeNode* TRNode; const CANavQuadTreeNode* BLNode; const CANavQuadTreeNode* BRNode;
    virtual void SaveToFile(CAFile& file, const CNodeIndexMap& nodeIndices, bool flag, const C2DVector& position) const
    {
        CANavQuadTreeNode::SaveToFile(file, nodeIndices, flag, position);
        long index;
        { CNodeIndexMap::const_iterator it = nodeIndices.find(TLNode); index = it->second; file.Write(&index, 4, false); }
        { CNodeIndexMap::const_iterator it = nodeIndices.find(TRNode); index = it->second; file.Write(&index, 4, false); }
        { CNodeIndexMap::const_iterator it = nodeIndices.find(BLNode); index = it->second; file.Write(&index, 4, false); }
        { CNodeIndexMap::const_iterator it = nodeIndices.find(BRNode); index = it->second; file.Write(&index, 4, false); }
        TLNode->SaveToFile(file, nodeIndices, false, position);
        TRNode->SaveToFile(file, nodeIndices, false, position);
        BLNode->SaveToFile(file, nodeIndices, false, position);
        BRNode->SaveToFile(file, nodeIndices, false, position);
    }
};
int main()
{
    CANavQuadTreeNode a, b, c, d; a.id = 1; b.id = 2; c.id = 3; d.id = 4;
    CNavQuadTreeNode root; root.id = 0; root.TLNode = &a; root.TRNode = &b; root.BLNode = &c; root.BRNode = &d;
    CNodeIndexMap m; m[&a] = 7; m[&b] = 9; m[&c] = 11; m[&d] = 13; m[&root] = 5;
    CAFile file; C2DVector pos = { 0, 0 };
    root.SaveToFile(file, m, true, pos);
    static const long expect[9] = { 1000, 7, 9, 11, 13, 1001, 1002, 1003, 1004 };
    if (g_nw != 9 || g_baseSaves != 5 || g_childFlags != 1) { std::printf("00a77d60_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (g_written[i] != expect[i]) { std::printf("00a77d60_TEST FAIL\n"); return 1; }
    std::printf("00a77d60_TEST PASS\n");
    return 0;
}
