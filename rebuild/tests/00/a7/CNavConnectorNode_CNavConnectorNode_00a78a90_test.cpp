#include <cstdio>
class CNavRegion { public: CNavRegion* connected; void SetAsConnectedToRegion(CNavRegion* o) { connected = o; } };
struct Node;
struct SmallVec {
    const Node* m_data[8]; signed char m_size;
    void resize(long n, const Node* const& fill = 0) { while (m_size < n) m_data[m_size++] = fill; m_size = (signed char)n; }
    void push_back(const Node* v) { resize(m_size + 1); m_data[m_size - 1] = v; }
};
struct Node { CNavRegion* region; SmallVec Neighbours; CNavRegion* GetRegion() const { return region; } };
class CNavConnectorNode { public:
    Node* Neighbour1; Node* Neighbour2;
    CNavConnectorNode(const Node* a, const Node* b) : Neighbour1((Node*)a), Neighbour2((Node*)b)
    {
        Neighbour1->Neighbours.push_back(Neighbour2);
        Neighbour2->Neighbours.push_back(Neighbour1);
        CNavRegion* region = Neighbour1->GetRegion();
        region->SetAsConnectedToRegion(Neighbour2->GetRegion());
    }
};
int main()
{
    CNavRegion r1, r2; r1.connected = r2.connected = 0;
    Node a, b, c; a.region = &r1; b.region = &r2; c.region = &r2;
    a.Neighbours.m_size = 0; b.Neighbours.m_size = 1; b.Neighbours.m_data[0] = &c;
    CNavConnectorNode n(&a, &b);
    if (n.Neighbour1 != &a || a.Neighbours.m_size != 1 || a.Neighbours.m_data[0] != &b || b.Neighbours.m_size != 2 ||
        b.Neighbours.m_data[0] != &c || b.Neighbours.m_data[1] != &a || r1.connected != &r2 || r2.connected != 0) { std::printf("00a78a90_TEST FAIL\n"); return 1; }
    std::printf("00a78a90_TEST PASS\n");
    return 0;
}