// Behaviour test for STLport map<CCharString, C2DVector>::operator[] (0x006bd6a0): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

struct Key {
    char s[16];
    Key() { s[0] = 0; }
    Key(const char* p) { std::strcpy(s, p); }
    bool operator<(const Key& o) const { return std::strcmp(s, o.s) < 0; }
};
struct V2 { float x, y; };

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
    Map<V2> m;
    m[Key("m")];
    { V2& r = m[Key("m")]; r.x = 1.0f; r.y = -1.0f; }
    { V2& r = m[Key("z")]; r.x = 2.0f; r.y = -2.0f; }
    { V2& r = m[Key("a")]; r.x = 3.0f; r.y = -3.0f; }
    if (!(m[Key("m")].x == 1.0f && m[Key("m")].y == -1.0f)) { std::printf("006bd6a0_TEST FAIL\n"); return 1; }
    if (!(m[Key("z")].x == 2.0f && m[Key("z")].y == -2.0f)) { std::printf("006bd6a0_TEST FAIL\n"); return 1; }
    if (!(m[Key("a")].x == 3.0f && m[Key("a")].y == -3.0f)) { std::printf("006bd6a0_TEST FAIL\n"); return 1; }
    if (m.count != 3) { std::printf("006bd6a0_TEST FAIL\n"); return 1; }
    if (std::strcmp(m.head->first.s, "a") || std::strcmp(m.head->next->first.s, "m") ||
        std::strcmp(m.head->next->next->first.s, "z")) { std::printf("006bd6a0_TEST FAIL\n"); return 1; }
    std::printf("006bd6a0_TEST PASS\n");
    return 0;
}