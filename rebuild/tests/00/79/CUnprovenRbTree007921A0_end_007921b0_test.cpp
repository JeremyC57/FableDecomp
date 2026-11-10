// Behaviour test for the red-black tree end() (0x007921B0): the past-the-end iterator is the tree's header node itself (while
// begin(), 0x007921A0, is the header's leftmost link).
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; };
struct Tree {
    struct iterator { Node* node; iterator(Node* n) : node(n) { } };
    Node* header;
    iterator begin() { return iterator(header->left); }
    iterator end() { return iterator(header); }
};
int main()
{
    Node header = { false, 0, 0, 0 };
    Node a = { true, &header, 0, 0 };
    header.left = &a; header.right = &a; header.parent = &a;
    Tree t; t.header = &header;
    bool ok = t.end().node == &header && t.begin().node == &a && t.end().node != t.begin().node;
    Node empty = { false, 0, 0, 0 }; empty.left = &empty; empty.right = &empty;
    Tree e; e.header = &empty;
    ok = ok && e.begin().node == e.end().node;
    if (!ok) { std::printf("007921b0_TEST FAIL\n"); return 1; }
    std::printf("007921b0_TEST PASS\n");
    return 0;
}