// Behaviour test for 0x0051AF80 _Rb_tree<long, pair<const long, list<long> > >::erase(first, last).
// _Rebalance_for_erase is modelled by the SGI unlink without the recolouring (the tree shape is irrelevant here).
#include <cstdio>
#include <cstdlib>

static int g_blocks = 0;
static void* Alloc(unsigned int n) { ++g_blocks; return std::malloc(n); }
static void Free(void* p) { if (p) { --g_blocks; std::free(p); } }

struct ListNode { ListNode* next; ListNode* prev; long data; };
struct Node { bool color; Node* parent; Node* left; Node* right; long key; ListNode* list; };
struct Tree { Node* header; unsigned int count; };

static Node* Minimum(Node* x) { while (x->left) x = x->left; return x; }
static Node* Maximum(Node* x) { while (x->right) x = x->right; return x; }

static Node* Unlink(Node* z, Node*& root, Node*& leftmost, Node*& rightmost)
{
    Node* y = z; Node* x = 0;
    if (y->left == 0) x = y->right;
    else if (y->right == 0) x = y->left;
    else { y = Minimum(y->right); x = y->right; }
    if (y != z) {
        z->left->parent = y; y->left = z->left;
        if (y != z->right) {
            if (x) x->parent = y->parent;
            y->parent->left = x; y->right = z->right; z->right->parent = y;
        }
        if (root == z) root = y; else if (z->parent->left == z) z->parent->left = y; else z->parent->right = y;
        y->parent = z->parent;
        y = z;
    } else {
        if (x) x->parent = y->parent;
        if (root == z) root = x; else if (z->parent->left == z) z->parent->left = x; else z->parent->right = x;
        if (leftmost == z) leftmost = z->right == 0 ? z->parent : Minimum(x);
        if (rightmost == z) rightmost = z->left == 0 ? z->parent : Maximum(x);
    }
    return y;
}

static Node* Increment(Node* n)
{
    if (n->right) { n = n->right; while (n->left) n = n->left; }
    else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}

static void DestroyList(ListNode* head)
{
    ListNode* cur = head->next;
    while (cur != head) { ListNode* tmp = cur; cur = cur->next; Free(tmp); }
    head->next = head; head->prev = head;
    Free(head);
}

static void EraseSubtree(Node* x)
{
    while (x) { EraseSubtree(x->right); Node* y = x->left; DestroyList(x->list); Free(x); x = y; }
}

static void EraseRange(Tree& t, Node* first, Node* last)
{
    if (first == t.header->left && last == t.header) {
        if (t.count != 0) {
            EraseSubtree(t.header->parent);
            t.header->left = t.header; t.header->parent = 0; t.header->right = t.header; t.count = 0;
        }
    } else {
        while (first != last) {
            Node* victim = first;
            first = Increment(first);
            Node* y = Unlink(victim, t.header->parent, t.header->left, t.header->right);
            DestroyList(y->list);
            Free(y);
            --t.count;
        }
    }
}

static Node* MakeNode(long key, Node* parent)
{
    Node* n = (Node*)Alloc(sizeof(Node));
    n->color = false; n->parent = parent; n->left = n->right = 0; n->key = key;
    n->list = (ListNode*)Alloc(sizeof(ListNode)); n->list->next = n->list->prev = n->list;
    for (long i = 0; i < key; ++i) {
        ListNode* e = (ListNode*)Alloc(sizeof(ListNode)); e->data = i;
        e->next = n->list; e->prev = n->list->prev; n->list->prev->next = e; n->list->prev = e;
    }
    return n;
}

static Node* Build(long lo, long hi, Node* parent)
{
    if (lo > hi) return 0;
    long mid = (lo + hi) / 2;
    Node* n = MakeNode(mid, parent);
    n->left = Build(lo, mid - 1, n);
    n->right = Build(mid + 1, hi, n);
    return n;
}

static void MakeTree(Tree& t)
{
    t.header = (Node*)Alloc(sizeof(Node));
    t.header->parent = Build(1, 7, t.header);
    t.header->left = Minimum(t.header->parent); t.header->right = Maximum(t.header->parent);
    t.count = 7;
}

static Node* Find(Tree& t, long key) { Node* n = t.header->left; while (n != t.header && n->key != key) n = Increment(n); return n; }

int main()
{
    int ok = 1;
    Tree t; MakeTree(t);
    EraseRange(t, Find(t, 3), Find(t, 6));               // erase keys 3, 4, 5
    if (t.count != 4) ok = 0;
    long want[4] = { 1, 2, 6, 7 }; int i = 0;
    for (Node* n = t.header->left; n != t.header; n = Increment(n), ++i) if (i >= 4 || n->key != want[i]) ok = 0;
    if (i != 4) ok = 0;
    EraseRange(t, Find(t, 1), Find(t, 2));               // erase the leftmost
    if (t.count != 3 || t.header->left->key != 2) ok = 0;
    EraseRange(t, Find(t, 7), t.header);                 // erase the rightmost (not the whole tree)
    if (t.count != 2 || t.header->right->key != 6) ok = 0;
    EraseRange(t, t.header, t.header);                   // empty range
    if (t.count != 2) ok = 0;
    EraseRange(t, t.header->left, t.header);             // the whole tree: clear()
    if (t.count != 0 || t.header->parent != 0 || t.header->left != t.header || t.header->right != t.header) ok = 0;
    Free(t.header);
    if (g_blocks != 0) ok = 0;
    if (!ok) { std::printf("0051af80_TEST FAIL\n"); return 1; }
    std::printf("0051af80_TEST PASS\n");
    return 0;
}