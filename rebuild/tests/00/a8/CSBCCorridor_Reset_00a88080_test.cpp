// Link test for 0x00A88080: the source object's CSBCCorridor::Reset (and its vtable) clears a three-wall STLport list of
// 24-byte CWall nodes and re-links the sentinel; the members behind the one-word list are untouched.
#include <cstdio>
#include <cstdlib>
class C2DVector { public: float X, Y; };
class CWall { public: C2DVector Point1; C2DVector Point2; C2DVector ToInternal; };
struct CRetailWallList {
    struct node { node* next; node* prev; CWall value; };
    node* m_pNode;
    void push_back(float x)
    {
        node* n = (node*)std::malloc(sizeof(node));
        n->value.Point1.X = x;
        n->next = m_pNode; n->prev = m_pNode->prev; m_pNode->prev->next = n; m_pNode->prev = n;
    }
};
class CSBCCorridor {
public:
    virtual void Reset();                      // from the source object (with the vtable)
    unsigned char   _base[0x14];
    CRetailWallList Walls;
    C2DVector       CurrentEndPoint;
    float           BestLeft;
    float           BestRight;
};
int main()
{
    int ok = sizeof(CWall) == 24 && sizeof(CRetailWallList::node) == 0x20 && sizeof(CSBCCorridor) == 0x2C;
    CSBCCorridor* c = new CSBCCorridor;
    c->Walls.m_pNode = (CRetailWallList::node*)std::malloc(sizeof(CRetailWallList::node));
    c->Walls.m_pNode->next = c->Walls.m_pNode; c->Walls.m_pNode->prev = c->Walls.m_pNode;
    c->BestLeft = 1.5f; c->BestRight = 2.5f;
    c->Walls.push_back(1.0f); c->Walls.push_back(2.0f); c->Walls.push_back(3.0f);
    ok = ok && c->Walls.m_pNode->next->next->next->value.Point1.X == 3.0f;
    c->Reset();
    ok = ok && c->Walls.m_pNode->next == c->Walls.m_pNode && c->Walls.m_pNode->prev == c->Walls.m_pNode;
    ok = ok && c->BestLeft == 1.5f && c->BestRight == 2.5f;
    c->Reset();                                // empty list: nothing to free
    ok = ok && c->Walls.m_pNode->next == c->Walls.m_pNode;
    std::free(c->Walls.m_pNode);
    delete c;
    if (!ok) { std::printf("00a88080_TEST FAIL\n"); return 1; }
    std::printf("00a88080_TEST PASS\n");
    return 0;
}
