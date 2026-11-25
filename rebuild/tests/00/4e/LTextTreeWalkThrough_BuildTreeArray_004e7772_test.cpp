// Behaviour model of the STLport _Rb_tree::_M_erase 0x004E7772 (map<EObjectAugmentationType, CObjectAugmentationParticleSet>): every
// node of the subtree is visited once -- the right subtree first, then the node's value is destroyed and the node freed, walking down
// the left spine.
#include <stdio.h>
#include <stdlib.h>
struct Node { int color; Node* parent; Node* left; Node* right; int key; int value; };
static int g_destroyed = 0, g_freed = 0, g_order_ok = 1, g_last_key = -1;
static void destroy_value(Node* n) { g_destroyed++; if (n->key < g_last_key) g_order_ok = 0; g_last_key = n->key; }
static void erase(Node* x)
{
    while (x != 0) {
        erase(x->right);
        Node* y = x->left;
        destroy_value(x);
        free(x);
        g_freed++;
        x = y;
    }
}
static Node* make(int key, Node* l, Node* r)
{
    Node* n = (Node*)malloc(sizeof(Node));
    n->color = 0; n->parent = 0; n->left = l; n->right = r; n->key = key; n->value = 0;
    return n;
}
int main(void)
{
    /* right-first, then left spine: keys laid out so the visit order is ascending */
    Node* root = make(2, make(4, 0, make(3, 0, 0)), make(0, make(1, 0, 0), 0));
    erase(root);
    if (g_destroyed != 5 || g_freed != 5 || !g_order_ok) { printf("FAIL %d %d %d\n", g_destroyed, g_freed, g_order_ok); return 1; }
    erase(0);
    if (g_destroyed != 5) { printf("FAIL null\n"); return 1; }
    printf("PASS_004e7772\n");
    return 0;
}
