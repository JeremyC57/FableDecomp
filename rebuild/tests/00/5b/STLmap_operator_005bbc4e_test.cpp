// Behaviour test for STLport map<EClothingSuitPart, bool>::operator[] (0x005bbc4e): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

enum EClothingSuitPart { SUIT_HEAD, SUIT_TORSO, SUIT_LEGS };
typedef EClothingSuitPart K;
inline bool Less(const K& a, const K& b) { return a < b; }
typedef bool T;

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

#define CHECK(c) if (!(c)) { std::printf("005bbc4e_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[SUIT_LEGS] == false);
    m[SUIT_TORSO] = true;
    CHECK(m[SUIT_TORSO] == true && m[SUIT_HEAD] == false && m.count == 3);
    CHECK(m.head->first == SUIT_HEAD && m.head->next->first == SUIT_TORSO);
    std::printf("005bbc4e_TEST PASS\n");
    return 0;
}