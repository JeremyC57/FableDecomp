// Behaviour test for STLport map<CCharString, CCharString>::operator[] (0x009ac700): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct Key {
    char s[16];
    Key() { s[0] = 0; }
    Key(const char* p) { std::strcpy(s, p); }
    bool operator<(const Key& o) const { return std::strcmp(s, o.s) < 0; }
};

template <class T> struct Node { Node* next; Key first; T second; };

template <class T> struct Map {
    Node<T>* head;                                  // sorted singly linked list stands in for the tree
    unsigned int count;
    Map() : head(0), count(0) {}
    ~Map() { while (head) { Node<T>* n = head->next; delete head; head = n; } }
    Node<T>** lower_bound(const Key& k)
    {
        Node<T>** p = &head;
        while (*p && (*p)->first < k) p = &(*p)->next;
        return p;
    }
    T& operator[](const Key& k)                     // STLport shape
    {
        Node<T>** i = lower_bound(k);
        if (*i == 0 || k < (*i)->first) {
            Node<T>* n = new Node<T>;
            n->first = k; n->second = T(); n->next = *i; *i = n; ++count;
        }
        return (*i)->second;
    }
};

int main()
{
    Map<Key> m;
    if (!(m[Key("m")].s[0] == 0)) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    m[Key("m")] = Key("1");
    m[Key("z")] = Key("2");
    m[Key("a")] = Key("3");
    if (!(std::strcmp(m[Key("m")].s, "1") == 0)) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    if (!(std::strcmp(m[Key("z")].s, "2") == 0)) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    if (!(std::strcmp(m[Key("a")].s, "3") == 0)) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    if (m.count != 3) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    if (std::strcmp(m.head->first.s, "a") || std::strcmp(m.head->next->first.s, "m") ||
        std::strcmp(m.head->next->next->first.s, "z")) { std::printf("009ac700_TEST FAIL\n"); return 1; }
    std::printf("009ac700_TEST PASS\n");
    return 0;
}