#include <cstdio>

// Model of the STLport _Rb_tree::_M_find over map<pair<long,long>, ...> (retail 0x00B2E830).
struct Key { long first; long second; };
static bool Less(const Key& x, const Key& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }

struct Node {
    bool  color;
    Node* parent;
    Node* left;
    Node* right;
    Key   key;
};

struct Tree {
    Node* header;
    Node* lower_bound(const Key& k) const
    {
        Node* y = header;
        Node* x = header->parent;
        while (x != 0)
            if (!Less(x->key, k))
                y = x, x = x->left;
            else
                x = x->right;
        return y;
    }
    Node* find(const Key& k) const
    {
        Node* y = lower_bound(k);
        if (y == header || Less(k, y->key))
            y = header;
        return y;
    }
};

static Node MakeNode(long a, long b) { Node n; n.color = false; n.parent = 0; n.left = 0; n.right = 0; n.key.first = a; n.key.second = b; return n; }

int main()
{
    Node header = MakeNode(0, 0);
    Node n1 = MakeNode(1, 5), n2 = MakeNode(2, 1), n3 = MakeNode(2, 9), n4 = MakeNode(4, 0);
    // BST: n2 root, n1 left, n4 right, n3 left of n4
    header.parent = &n2;
    n2.left = &n1; n2.right = &n4; n4.left = &n3;
    Tree t; t.header = &header;
    Key k;
    k.first = 0; k.second = 0; if (t.find(k) != &header) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 1; k.second = 5; if (t.find(k) != &n1) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 2; k.second = 1; if (t.find(k) != &n2) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 2; k.second = 9; if (t.find(k) != &n3) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 2; k.second = 5; if (t.find(k) != &header) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 4; k.second = 0; if (t.find(k) != &n4) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    k.first = 9; k.second = 0; if (t.find(k) != &header) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    Node empty = MakeNode(0, 0); Tree e; e.header = &empty;
    if (e.find(k) != &empty) { std::printf("00b2e630_TEST FAIL\n"); return 1; }
    std::printf("00b2e630_TEST PASS\n");
    return 0;
}