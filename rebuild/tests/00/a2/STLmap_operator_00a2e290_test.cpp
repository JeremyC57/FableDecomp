// Behaviour test for STLport map<CCharString, vector<CMeshDataBank::CLipSyncEntry>>::operator[] (0x00a2e290): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct K {
    char s[16];
    K() { s[0] = 0; }
    K(const char* p) { std::strcpy(s, p); }
};
inline bool Less(const K& a, const K& b) { return std::strcmp(a.s, b.s) < 0; }
struct CLipSyncEntry { K PhonemeName; unsigned int AnimationIndex; };

template <class E> struct Vec {                     // vector-like value: empty by default
    E items[8];
    unsigned int size;
    Vec() : size(0) {}
    void push_back(const E& e) { items[size++] = e; }
};
typedef Vec<CLipSyncEntry> T;

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

#define CHECK(c) if (!(c)) { std::printf("00a2e290_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[K("m")].size == 0);
    CHECK(m.count == 1);
    CLipSyncEntry e; e.PhonemeName = K("ah"); e.AnimationIndex = 4;
    m[K("m")].push_back(e);
    e.AnimationIndex = 5; m[K("z")].push_back(e);
    e.AnimationIndex = 6; m[K("a")].push_back(e);
    CHECK(m[K("m")].items[0].AnimationIndex == 4 && std::strcmp(m[K("m")].items[0].PhonemeName.s, "ah") == 0);
    CHECK(m[K("z")].items[0].AnimationIndex == 5 && m[K("a")].items[0].AnimationIndex == 6);
    CHECK(m.count == 3);
    CHECK(std::strcmp(m.head->first.s, "a") == 0 && std::strcmp(m.head->next->first.s, "m") == 0 &&
          std::strcmp(m.head->next->next->first.s, "z") == 0);
    std::printf("00a2e290_TEST PASS\n");
    return 0;
}