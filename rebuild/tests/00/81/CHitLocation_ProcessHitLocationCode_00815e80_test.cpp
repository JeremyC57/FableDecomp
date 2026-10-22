#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_a = 0, g_b = 0, g_key = 0; static void Handler(long a, long b) { g_a = a; g_b = b; }
struct CRetailTree { struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; void (*value)(long, long); }; struct iterator { node* current; iterator(node* n) : current(n) { } }; node* head; unsigned long size;
    iterator find(const long& key) { g_key = key; node* result = head; node* n = head->parent; while (n) { if (!(n->key < key)) { result = n; n = n->left; } else n = n->right; } node* found = result; if (found == head || key < found->key) found = head; return iterator(found); } };
static CRetailTree g_tree; struct COwner { unsigned char _pad[0x78]; long m_code; };
struct CHitLocation { long POwnerThing; COwner* m_pOwner; void ProcessHitLocationCode(long a, long b) { CRetailTree::iterator it = g_tree.find(m_pOwner->m_code); if (it.current != g_tree.head) it.current->value(a, b); } };

int main() {
    CRetailTree::node head, n; g_tree.head = &head; g_tree.size = 1; head.parent = &n; n.left = n.right = 0; n.key = 77; n.value = Handler; COwner o; o.m_code = 77; CHitLocation l; l.m_pOwner = &o;
    l.ProcessHitLocationCode(3, 4); if (g_a != 3 || g_b != 4 || g_key != 77) { std::printf("00815e80_TEST FAIL\n"); return 1; } o.m_code = 78; g_a = 0; l.ProcessHitLocationCode(5, 6); if (g_a != 0 || g_key != 78) { std::printf("00815e80_TEST FAIL\n"); return 1; }
    std::printf("00815e80_TEST PASS\n"); return 0;
}