// Behaviour test for STLport map<long, CEngineSceneRenderStats>::operator[] (0x00b97b60): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

typedef long K;
inline bool Less(const K& a, const K& b) { return a < b; }
struct T {
    unsigned int StaticPrimitivesRendered, MovingPrimitivesRendered, StaticPrimitivesTouched, MovingPrimitivesTouched,
                 VisibleCells, BoundingBoxTests, BoundingSphereTests;
    T() : StaticPrimitivesRendered(0), MovingPrimitivesRendered(0), StaticPrimitivesTouched(0), MovingPrimitivesTouched(0),
          VisibleCells(0), BoundingBoxTests(0), BoundingSphereTests(0) {}
};

struct Node { Node* next; K first; T second; };

struct Map {
    Node* head;                                     // sorted singly linked list stands in for the tree
    unsigned int count;
    Map() : head(0), count(0) {}
    ~Map() { while (head) { Node* n = head->next; delete head; head = n; } }
    Node** lower_bound(const K& k)
    {
        Node** p = &head;
        while (*p && Less((*p)->first, k)) p = &(*p)->next;
        return p;
    }
    T& operator[](const K& k)                       // STLport shape
    {
        Node** i = lower_bound(k);
        if (*i == 0 || Less(k, (*i)->first)) {
            Node* n = new Node;
            n->first = k; n->second = T(); n->next = *i; *i = n; ++count;
        }
        return (*i)->second;
    }
};

#define CHECK(c) if (!(c)) { std::printf("00b97b60_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[1].VisibleCells == 0 && m[1].BoundingSphereTests == 0);
    m[1].VisibleCells += 4;
    m[0].BoundingBoxTests += 2;
    m[1].VisibleCells += 3;
    CHECK(m[1].VisibleCells == 7 && m[0].BoundingBoxTests == 2 && m.count == 2 && m.head->first == 0);
    std::printf("00b97b60_TEST PASS\n");
    return 0;
}