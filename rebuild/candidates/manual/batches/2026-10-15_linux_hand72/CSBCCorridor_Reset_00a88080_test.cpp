#include <cstdio>
#include <cstdlib>
// Behaviour model of 0x00A88080: CSBCCorridor::Reset frees every wall node and re-links the sentinel.
static int g_freed = 0;
static void FreeNode(void* p) { ++g_freed; std::free(p); }
struct CWall { float m_start[2]; float m_end[2]; };
struct CRetailWallList {
    struct node { node* next; node* prev; CWall value; };
    node* m_pNode;
    void clear()
    {
        node* n = m_pNode->next;
        while (n != m_pNode) { node* victim = n; n = n->next; FreeNode(victim); }
        m_pNode->next = m_pNode; m_pNode->prev = m_pNode;
    }
    void push_back()
    {
        node* n = (node*)std::malloc(sizeof(node));
        n->next = m_pNode; n->prev = m_pNode->prev; m_pNode->prev->next = n; m_pNode->prev = n;
    }
};
struct CSBCCorridor {
    virtual ~CSBCCorridor() {}
    CRetailWallList Walls;
    virtual void Reset() { Walls.clear(); }
};
int main()
{
    CSBCCorridor c; c.Walls.m_pNode = (CRetailWallList::node*)std::malloc(sizeof(CRetailWallList::node));
    c.Walls.m_pNode->next = c.Walls.m_pNode; c.Walls.m_pNode->prev = c.Walls.m_pNode;
    c.Walls.push_back(); c.Walls.push_back(); c.Walls.push_back();
    c.Reset();
    if (g_freed != 3 || c.Walls.m_pNode->next != c.Walls.m_pNode || c.Walls.m_pNode->prev != c.Walls.m_pNode)
        { std::printf("00a88080_TEST FAIL\n"); return 1; }
    c.Reset();                                 // empty list: nothing freed
    if (g_freed != 3) { std::printf("00a88080_TEST FAIL\n"); return 1; }
    std::free(c.Walls.m_pNode);
    std::printf("00a88080_TEST PASS\n");
    return 0;
}
