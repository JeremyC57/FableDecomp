// Behaviour test for STLport map<CCharString, vector<unsigned long>>::operator[] (0x009d4810): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct K {
    char s[16];
    K() { s[0] = 0; }
    K(const char* p) { std::strcpy(s, p); }
};
inline bool Less(const K& a, const K& b) { return std::strcmp(a.s, b.s) < 0; }

template <class E> struct Vec {                     // vector-like value: empty by default
    E items[8];
    unsigned int size;
    Vec() : size(0) {}
    void push_back(const E& e) { items[size++] = e; }
};
typedef Vec<unsigned int> T;

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

#define CHECK(c) if (!(c)) { std::printf("009d4810_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[K("m")].size == 0);
    CHECK(m.count == 1);
    for (unsigned int i = 0; i < 3; ++i) m[K("m")].push_back(i);
    m[K("z")].push_back(7u);
    m[K("a")].push_back(9u);
    CHECK(m[K("m")].size == 3 && m[K("m")].items[2] == 2u);
    CHECK(m[K("z")].items[0] == 7u && m[K("a")].items[0] == 9u);
    CHECK(m.count == 3);
    CHECK(std::strcmp(m.head->first.s, "a") == 0 && std::strcmp(m.head->next->first.s, "m") == 0 &&
          std::strcmp(m.head->next->next->first.s, "z") == 0);
    std::printf("009d4810_TEST PASS\n");
    return 0;
}