// Behaviour test for 0x0051BF90 map<CThingBuilding*, list<CThingAICreature*> >::operator[]: absent keys get an empty list.
#include <cstdio>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
template <class K, class V> struct Node : NodeBase { K key; V value; };
template <class K, class V> struct Tree {
    NodeBase* header; unsigned int count;
    void init() { header = new Node<K, V>(); header->color = false; header->parent = 0; header->left = header; header->right = header; count = 0; }
    NodeBase* lower_bound(const K& k) const
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x != 0) { if (!(((Node<K, V>*)x)->key < k)) { y = x; x = x->left; } else x = x->right; }
        return y;
    }
    // plain unbalanced BST insert standing in for insert_unique
    NodeBase* insert_unique(const K& k, const V& v)
    {
        Node<K, V>* z = new Node<K, V>(); z->key = k; z->value = v; z->left = z->right = 0;
        NodeBase** link = &header->parent; NodeBase* parent = header;
        while (*link) { parent = *link; link = (k < ((Node<K, V>*)parent)->key) ? &parent->left : &parent->right; }
        *link = z; z->parent = parent; ++count;
        if (header->left == header || k < ((Node<K, V>*)header->left)->key) header->left = z;
        if (header->right == header || ((Node<K, V>*)header->right)->key < k) header->right = z;
        return z;
    }
    V& subscript(const K& k)
    {
        NodeBase* i = lower_bound(k);
        if (i == header || k < ((Node<K, V>*)i)->key)
            i = insert_unique(k, V());
        return ((Node<K, V>*)i)->value;
    }
};
struct LNode { LNode* next; LNode* prev; void* data; };
struct List {
    LNode* s;
    List() { s = new LNode(); s->next = s; s->prev = s; }
    List(const List& o) { s = new LNode(); s->next = s; s->prev = s; for (LNode* p = o.s->next; p != o.s; p = p->next) push_back(p->data); }
    List& operator=(const List& o) { if (this != &o) { clear(); for (LNode* p = o.s->next; p != o.s; p = p->next) push_back(p->data); } return *this; }
    ~List() { clear(); delete s; }
    void clear() { LNode* p = s->next; while (p != s) { LNode* n = p->next; delete p; p = n; } s->next = s; s->prev = s; }
    void push_back(void* d) { LNode* n = new LNode(); n->data = d; n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n; }
    int size() const { int c = 0; for (LNode* p = s->next; p != s; p = p->next) ++c; return c; }
};
int main()
{
    Tree<void*, List> t; t.init();
    int b1, b2, c1, c2;
    List& l1 = t.subscript(&b1);
    if (l1.size() != 0 || t.count != 1) { std::printf("0051bf90_TEST FAIL\n"); return 1; }
    l1.push_back(&c1); l1.push_back(&c2);
    t.subscript(&b2).push_back(&c1);
    if (t.subscript(&b1).size() != 2 || t.subscript(&b2).size() != 1 || t.count != 2) { std::printf("0051bf90_TEST FAIL\n"); return 1; }
    std::printf("0051bf90_TEST PASS\n");
    return 0;
}