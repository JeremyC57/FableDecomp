// Behaviour test for 005bad5b: std::_Rb_tree<EClothingSuitPart,..>::insert_unique -- new keys inserted (true), duplicates rejected (false) with the existing node returned
#include <cstdio>
#include <cstdlib>
enum EClothingSuitPart { PART_0, PART_1, PART_2, PART_3, PART_4, PART_5 };
struct Node { bool color; Node* parent; Node* left; Node* right; EClothingSuitPart value; };
struct Iter { Node* node; Iter(Node* n) : node(n) {} };
struct Result { Iter first; bool second; Result(Iter f, bool s) : first(f), second(s) {} };
static Node* Decrement(Node* x)
{
    if (x->color == false && x->parent->parent == x) return x->right;
    if (x->left) { Node* y = x->left; while (y->right) y = y->right; return y; }
    Node* y = x->parent; while (x == y->left) { x = y; y = y->parent; } return y;
}
struct Tree {
    Node* header; unsigned int count;
    Iter Insert(Node* x, Node* y, const EClothingSuitPart& v)
    {
        Node* z = (Node*)std::malloc(sizeof(Node)); z->value = v; z->left = 0; z->right = 0; z->parent = y; z->color = true;
        if (y == header || x != 0 || v < y->value) { y->left = z; if (y == header) { header->parent = z; header->right = z; } else if (y == header->left) header->left = z; }
        else { y->right = z; if (y == header->right) header->right = z; }
        ++count; return Iter(z);
    }
    Result insert_unique(const EClothingSuitPart& v)
    {
        Node* y = header; Node* x = header->parent; bool comp = true;
        while (x) { y = x; comp = v < x->value; x = comp ? x->left : x->right; }
        Iter j(y);
        if (comp) { if (j.node == header->left) return Result(Insert(y, y, v), true); j.node = Decrement(j.node); }
        if (j.node->value < v) return Result(Insert(x, y, v), true);
        return Result(j, false);
    }
};
int main()
{
    Node h; h.color = false; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0;
    EClothingSuitPart k3 = PART_3, k1 = PART_1, k5 = PART_5, k2 = PART_2;
    Result r1 = t.insert_unique(k3); Result r2 = t.insert_unique(k1); Result r3 = t.insert_unique(k5);
    Result r4 = t.insert_unique(k3); Result r5 = t.insert_unique(k1); Result r6 = t.insert_unique(k2);
    int ok = r1.second && r2.second && r3.second && !r4.second && r4.first.node == r1.first.node && !r5.second &&
             r5.first.node == r2.first.node && r6.second && t.count == 4 && h.left->value == PART_1 && h.right->value == PART_5;
    std::printf(ok ? "005bad5b_TEST PASS\n" : "005bad5b_TEST FAIL\n");
    return ok ? 0 : 1;
}
