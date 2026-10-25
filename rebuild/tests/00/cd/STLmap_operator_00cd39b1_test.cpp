// Behaviour test for STLport map<CCharString, unsigned long>::operator[] (0x00cd39b1): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct K {
    char s[16];
    K() { s[0] = 0; }
    K(const char* p) { std::strcpy(s, p); }
};
inline bool Less(const K& a, const K& b) { return std::strcmp(a.s, b.s) < 0; }
typedef unsigned int T;

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

#define CHECK(c) if (!(c)) { std::printf("00cd39b1_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[K("exit")] == 0u);
    m[K("exit")] = 12u;
    m[K("bridge")] = 3u;
    CHECK(m[K("exit")] == 12u && m[K("bridge")] == 3u && m.count == 2);
    CHECK(std::strcmp(m.head->first.s, "bridge") == 0);
    std::printf("00cd39b1_TEST PASS\n");
    return 0;
}