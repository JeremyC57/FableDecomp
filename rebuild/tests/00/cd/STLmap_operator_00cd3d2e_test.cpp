// Behaviour test for STLport map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] (0x00cd3d2e): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct K {
    char s[16];
    K() { s[0] = 0; }
    K(const char* p) { std::strcpy(s, p); }
};
inline bool Less(const K& a, const K& b) { return std::strcmp(a.s, b.s) < 0; }
struct T {                                          // scripted-thing handle: null counted pointer by default
    void* Data; long* Info;
    T() : Data(0), Info(0) {}
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

#define CHECK(c) if (!(c)) { std::printf("00cd3d2e_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    CHECK(m[K("hero")].Data == 0 && m[K("hero")].Info == 0);
    long count = 1; int thing = 5;
    m[K("hero")].Data = &thing; m[K("hero")].Info = &count;
    CHECK(m[K("guard")].Data == 0);
    CHECK(m[K("hero")].Data == &thing && m.count == 2);
    CHECK(std::strcmp(m.head->first.s, "guard") == 0);
    std::printf("00cd3d2e_TEST PASS\n");
    return 0;
}