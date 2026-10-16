// behaviour test for 0x004C0430 CTCGraphicAppearance::InheritMeshEffectsFromCarrier (in-order walk of an
// STLport-shaped red-black tree with a header node, copying entries that affect carried items)
#include <cstdio>
struct Node { Node* parent; Node* left; Node* right; int key; bool carried; bool weapons; };
static Node* Increment(Node* x)
{
    if (x->right != 0) { x = x->right; while (x->left != 0) x = x->left; }
    else { Node* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
int main()
{
    // header H: parent = root, left = leftmost, right = rightmost; tree 1 <- 2 -> 3
    Node H, n1, n2, n3;
    n2.parent = &H; n2.left = &n1; n2.right = &n3; n2.key = 2; n2.carried = false; n2.weapons = false;
    n1.parent = &n2; n1.left = 0; n1.right = 0; n1.key = 1; n1.carried = true; n1.weapons = true;
    n3.parent = &n2; n3.left = 0; n3.right = 0; n3.key = 3; n3.carried = true; n3.weapons = false;
    H.parent = &n2; H.left = &n1; H.right = &n3; H.key = 0;
    int applied[4]; int count = 0; bool weap[4];
    for (Node* it = H.left; it != &H; it = Increment(it))
        if (it->carried) { weap[count] = it->weapons; applied[count++] = it->key; }
    bool ok = count == 2 && applied[0] == 1 && applied[1] == 3 && weap[0] && !weap[1];
    if (!ok) { std::printf("004c0430_TEST FAIL\n"); return 1; }
    std::printf("004c0430_TEST PASS\n");
    return 0;
}