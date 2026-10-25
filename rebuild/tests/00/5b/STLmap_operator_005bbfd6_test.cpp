// Behaviour test for STLport map<long, CEnginePrimitiveHandle>::operator[] (0x005bbfd6): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

typedef long K;
inline bool Less(const K& a, const K& b) { return a < b; }
struct T { void* Primitive; T() : Primitive(0) {} };   // empty primitive handle by default

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

#define CHECK(c) if (!(c)) { std::printf("005bbfd6_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    int prim = 0;
    CHECK(m[7].Primitive == 0);
    m[7].Primitive = &prim;
    CHECK(m[-2].Primitive == 0 && m[7].Primitive == &prim && m.count == 2);
    CHECK(m.head->first == -2);
    std::printf("005bbfd6_TEST PASS\n");
    return 0;
}