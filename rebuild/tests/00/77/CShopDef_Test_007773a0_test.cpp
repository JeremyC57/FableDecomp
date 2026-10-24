// Behaviour test for 0x007773A0 CShopDef::Test() const -- the in-order STLport tree walk over both attitude maps
#include <cstdio>

struct Node {
    bool Color;
    Node* Parent;
    Node* Left;
    Node* Right;
    int Key;
};

static Node* Increment(Node* node)
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    }
    else {
        Node* y = node->Parent;
        while (node == y->Right) {
            node = y;
            y = y->Parent;
        }
        if (node->Right != y)
            node = y;
    }
    return node;
}

static int Walk(Node* header, int* keys)
{
    int n = 0;
    for (Node* it = header->Left; it != header; it = Increment(it))
        keys[n++] = it->Key;
    return n;
}

int main()
{
    // empty map: header's left/right point at the header itself
    Node empty = { true, 0, 0, 0, -1 };
    empty.Left = &empty; empty.Right = &empty;
    int keys[8];
    if (Walk(&empty, keys) != 0) { std::printf("007773a0_TEST FAIL\n"); return 1; }

    // three-node tree 1 < 2 < 3 rooted at 2 (STLport header: parent = root, left = min, right = max)
    Node header = { true, 0, 0, 0, -1 };
    Node n1 = { false, 0, 0, 0, 1 }, n2 = { true, 0, 0, 0, 2 }, n3 = { false, 0, 0, 0, 3 };
    header.Parent = &n2; header.Left = &n1; header.Right = &n3;
    n2.Parent = &header; n2.Left = &n1; n2.Right = &n3;
    n1.Parent = &n2; n3.Parent = &n2;
    int n = Walk(&header, keys);
    if (n != 3 || keys[0] != 1 || keys[1] != 2 || keys[2] != 3) { std::printf("007773a0_TEST FAIL\n"); return 1; }
    std::printf("007773a0_TEST PASS\n");
    return 0;
}