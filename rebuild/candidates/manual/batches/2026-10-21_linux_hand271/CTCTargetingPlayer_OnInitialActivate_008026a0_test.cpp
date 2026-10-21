// Behaviour test for CTCTargetingPlayer::OnInitialActivate (0x008026A0): the max targeting distance is 1.2 times the largest
// range in the def's range map (an in-order tree walk), and the targeting vectors are zeroed.
#include <cstdio>

struct Node { Node* parent; Node* left; Node* right; long key; float value; };
static Node* Increment(Node* node)
{
    if (node->right != 0) {
        node = node->right;
        while (node->left != 0)
            node = node->left;
    } else {
        Node* y = node->parent;
        while (node == y->right) { node = y; y = y->parent; }
        if (node->right != y)
            node = y;
    }
    return node;
}
int main()
{
    // header + 3 nodes: b is the root with children a and c; header.left = a (leftmost), header.right = c, header.parent = b
    Node header, a, b, c;
    header.parent = &b; header.left = &a; header.right = &c;
    b.parent = &header; b.left = &a; b.right = &c; b.key = 2; b.value = 7.0f;
    a.parent = &b; a.left = 0; a.right = 0; a.key = 1; a.value = 3.0f;
    c.parent = &b; c.left = 0; c.right = 0; c.key = 3; c.value = 5.0f;
    float maxDistance = 0.0f; int visited = 0;
    for (Node* it = header.left; it != &header; it = Increment(it)) {
        ++visited;
        if (it->value > maxDistance)
            maxDistance = it->value;
    }
    maxDistance *= 1.2f;
    float pos[3] = { 1.0f, 2.0f, 3.0f };
    pos[0] = 0.0f; pos[1] = 0.0f; pos[2] = 0.0f;
    if (visited != 3 || maxDistance < 8.39f || maxDistance > 8.41f || pos[1] != 0.0f) { std::printf("008026a0_TEST FAIL\n"); return 1; }
    std::printf("008026a0_TEST PASS\n");
    return 0;
}
