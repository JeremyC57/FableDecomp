// Behaviour test for CNavQuadTreeNode::~CNavQuadTreeNode (0x00A75C20): delete each child (TL, TR, BR, BL) unless it is null or
// the shared blocked node, clear every pointer, then the base destructor.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CNodeModel { const char* Tag; CNodeModel(const char* t) : Tag(t) { } virtual ~CNodeModel() { Log(Tag); } };
static CNodeModel g_BlockedNode("X");
struct CNavQuadTreeNodeModel : CNodeModel {
    CNodeModel* TLNode; CNodeModel* TRNode; CNodeModel* BLNode; CNodeModel* BRNode;
    CNodeModel** Seen[4];
    CNavQuadTreeNodeModel() : CNodeModel("|") { }
    virtual ~CNavQuadTreeNodeModel()
    {
        if (TLNode && TLNode != &g_BlockedNode) delete TLNode;
        TLNode = 0;
        if (TRNode && TRNode != &g_BlockedNode) delete TRNode;
        TRNode = 0;
        if (BRNode && BRNode != &g_BlockedNode) delete BRNode;
        BRNode = 0;
        if (BLNode && BLNode != &g_BlockedNode) delete BLNode;
        BLNode = 0;
        if (TLNode || TRNode || BLNode || BRNode) Log("!");
    }
};
int main()
{
    {
        CNavQuadTreeNodeModel n;
        n.TLNode = new CNodeModel("a");
        n.TRNode = &g_BlockedNode;
        n.BLNode = new CNodeModel("c");
        n.BRNode = 0;
    }
    if (std::strcmp(g_log, "ac|") != 0) { std::printf("00a75c20_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    {
        CNavQuadTreeNodeModel n;
        n.TLNode = new CNodeModel("1"); n.TRNode = new CNodeModel("2"); n.BLNode = new CNodeModel("3"); n.BRNode = new CNodeModel("4");
    }
    if (std::strcmp(g_log, "1243|") != 0) { std::printf("00a75c20_TEST FAIL\n"); return 1; }
    std::printf("00a75c20_TEST PASS\n");
    return 0;
}