#include <cstdio>
// Behaviour test for CNavigatorManager::GetQuadTreeMemoryUsage (0x00A15100): sums each tree's usage, trees add to the counters.
class CNavQuadTree {
public:
    unsigned int Size;
    unsigned int GetMemoryUsage(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d) const
    { a += Size; b += 1; c += 2; d += 3; return Size * 10; }
};
struct Map { CNavQuadTree* PQuadTree; void* Info; int Index; };
struct Node { Node* _M_next; Node* _M_prev; Map _M_data; };
class CNavigatorManager {
public:
    Node* QuadTrees;
    unsigned int GetQuadTreeMemoryUsage(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d) const
    {
        unsigned int total = 0;
        for (Node* it = QuadTrees->_M_next; it != QuadTrees; it = it->_M_next)
            total += it->_M_data.PQuadTree->GetMemoryUsage(a, b, c, d);
        return total;
    }
};
int main()
{
    CNavQuadTree t1, t2; t1.Size = 4; t2.Size = 6;
    Node head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head;
    n1._M_data.PQuadTree = &t1; n2._M_data.PQuadTree = &t2;
    CNavigatorManager m; m.QuadTrees = &head;
    unsigned int a = 0, b = 0, c = 0, d = 0;
    unsigned int total = m.GetQuadTreeMemoryUsage(a, b, c, d);
    bool ok = total == 100 && a == 10 && b == 2 && c == 4 && d == 6;
    Node empty; empty._M_next = &empty; m.QuadTrees = &empty;
    ok = ok && m.GetQuadTreeMemoryUsage(a, b, c, d) == 0 && a == 10;
    if (!ok) { std::printf("00a15100_TEST FAIL\n"); return 1; }
    std::printf("00a15100_TEST PASS\n");
    return 0;
}