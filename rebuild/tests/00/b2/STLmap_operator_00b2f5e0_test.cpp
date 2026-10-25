// Behaviour test for STLport map<pair<long, long>, CEngineTextureMorph>::operator[] (0x00b2f5e0): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key (pair keys: first, then second).
#include <cstdio>
#include <cstring>

struct K { long first, second; K() : first(0), second(0) {} K(long a, long b) : first(a), second(b) {} };
inline bool Less(const K& x, const K& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }
struct T { long OriginalTextureIndex, MorphID; unsigned int BlendCount; T() : OriginalTextureIndex(0), MorphID(0), BlendCount(0) {} };

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

#define CHECK(c) if (!(c)) { std::printf("00b2f5e0_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[K(2, 5)].MorphID == 0 && m[K(2, 5)].BlendCount == 0);
    m[K(2, 5)].MorphID = 11;
    m[K(2, 1)].MorphID = 12;
    m[K(1, 9)].MorphID = 13;
    CHECK(m.count == 3);
    CHECK(m.head->second.MorphID == 13 && m.head->next->second.MorphID == 12 && m.head->next->next->second.MorphID == 11);
    CHECK(m[K(2, 5)].MorphID == 11 && m.count == 3);
    std::printf("00b2f5e0_TEST PASS\n");
    return 0;
}