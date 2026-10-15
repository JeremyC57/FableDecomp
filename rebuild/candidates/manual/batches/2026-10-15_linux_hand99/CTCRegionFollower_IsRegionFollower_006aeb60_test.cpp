#include <cstdio>
struct CThing { int id; };
struct CBaseIntelligentPointer {
    CThing* PData;
    CThing* GetPItem() const { return PData; }
};
template<class T>
struct CRetailList {
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* m_node;
        iterator(node* n) : m_node(n) { }
        T& operator*() const { return m_node->value; }
        iterator& operator++() { m_node = m_node->next; return *this; }
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    node* head;
    iterator begin() const { return iterator(head->next); }
    iterator end() const { return iterator(head); }
};
struct CTCRegionFollower {
    CRetailList<CBaseIntelligentPointer> Followers;
    bool IsRegionFollower(CThing* thing)
    {
        if (thing) {
            for (CRetailList<CBaseIntelligentPointer>::iterator it = Followers.begin(); it != Followers.end(); ++it) {
                if ((*it).GetPItem() == thing)
                    return true;
            }
        }
        return false;
    }
};
int main()
{
    typedef CRetailList<CBaseIntelligentPointer>::node node;
    CThing a, b, c; a.id = 1; b.id = 2; c.id = 3;
    node head, n1, n2;
    CTCRegionFollower f; f.Followers.head = &head;
    head.next = &head; head.prev = &head;
    int bad = 0;
    if (f.IsRegionFollower(&a)) bad = 1;                 // empty list
    head.next = &n1; n1.next = &n2; n2.next = &head; head.prev = &n2; n2.prev = &n1; n1.prev = &head;
    n1.value.PData = &a; n2.value.PData = 0;
    if (!f.IsRegionFollower(&a)) bad = 1;
    if (f.IsRegionFollower(&b)) bad = 1;
    if (f.IsRegionFollower(0)) bad = 1;                  // null never matches the empty pointer
    n2.value.PData = &c;
    if (!f.IsRegionFollower(&c)) bad = 1;
    if (bad) { std::printf("006aeb60_TEST FAIL\n"); return 1; }
    std::printf("006aeb60_TEST PASS\n");
    return 0;
}
