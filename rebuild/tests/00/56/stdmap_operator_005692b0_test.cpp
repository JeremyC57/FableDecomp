// Behaviour test for 0x005692B0 map::operator[]: lower_bound, insert (k, 0) when absent, return the mapped value.
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
int main()
{
    Tree<int, float> t; t.init();
    t.subscript(5) = (float)50; t.subscript(2) = (float)20; t.subscript(9) = (float)90;
    if (t.count != 3 || t.subscript(5) != (float)50 || t.subscript(2) != (float)20 || t.subscript(9) != (float)90 || t.count != 3) {
        std::printf("005692b0_TEST FAIL\n"); return 1;
    }
    if (t.subscript(7) != (float)0 || t.count != 4) { std::printf("005692b0_TEST FAIL\n"); return 1; }
    t.subscript(7) += (float)3;
    if (t.subscript(7) != (float)3) { std::printf("005692b0_TEST FAIL\n"); return 1; }
    std::printf("005692b0_TEST PASS\n");
    return 0;
}