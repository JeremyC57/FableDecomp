// Behaviour test for 0x008814C0 NScreenEffect::CScreenEffectPriorityStack::~CScreenEffectPriorityStack()
// The model mirrors the source: STLport list / set member destructors run in reverse member order; every list entry is
// destroyed through its virtual destructor and every node, tree node and sentinel is freed.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_entryDtors = 0;
static int g_allocs = 0;
static int g_frees = 0;
static int g_order[16];
static int g_orderCount = 0;

static void* xmalloc(unsigned int n) { ++g_allocs; return std::malloc(n); }
static void xfree(void* p) { if (p) { ++g_frees; std::free(p); } }

struct CStackEntry { int m_tag; CStackEntry(int t) : m_tag(t) { } virtual ~CStackEntry() { ++g_entryDtors; } };
struct CEntryA : CStackEntry { CEntryA(int t) : CStackEntry(t) { } };

struct ListNode { ListNode* next; ListNode* prev; };
template <class T> struct NodeT : ListNode { T data; NodeT(const T& v) : data(v) { } };

template <class T> struct List {
    ListNode* head;
    int id;
    List() : id(0) { head = (ListNode*)xmalloc(sizeof(ListNode)); head->next = head; head->prev = head; }
    void push(int tag)
    {
        NodeT<T>* n = (NodeT<T>*)xmalloc(sizeof(NodeT<T>));
        new (&n->data) T(tag);
        n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
    }
    ~List()
    {
        g_order[g_orderCount++] = id;
        ListNode* cur = head->next;
        while (cur != head) {
            ListNode* tmp = cur;
            cur = cur->next;
            ((NodeT<T>*)tmp)->data.~T();
            xfree(tmp);
        }
        head->next = head; head->prev = head;
        xfree(head);
    }
};

struct TreeNode { char color; TreeNode* parent; TreeNode* left; TreeNode* right; void* value; };
struct Set {
    TreeNode* header;
    unsigned int count;
    int id;
    Set() : count(0), id(0) { header = (TreeNode*)xmalloc(sizeof(TreeNode)); header->parent = 0; header->left = header; header->right = header; }
    void erase(TreeNode* x) { while (x) { erase(x->right); TreeNode* y = x->left; xfree(x); x = y; } }
    void insertChain(int n)          // a degenerate left chain is still a valid tree for erase()
    {
        TreeNode* parent = header;
        for (int i = 0; i < n; ++i) {
            TreeNode* t = (TreeNode*)xmalloc(sizeof(TreeNode)); t->left = 0; t->right = 0; t->parent = parent;
            if (parent == header) header->parent = t; else parent->left = t;
            parent = t; ++count;
        }
    }
    ~Set()
    {
        g_order[g_orderCount++] = id;
        if (count != 0) { erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; }
        xfree(header);
    }
};

struct Stack {
    List<CEntryA> Radial[3];
    Set RadialKeys;
    List<CEntryA> Displacement[3];
    Set DisplacementKeys;
    List<CEntryA> Filter[3];
    Set FilterKeys;
    virtual ~Stack() { }
};

int main()
{
    int fail = 0;
    Stack* s = new Stack;
    for (int i = 0; i < 3; ++i) { s->Radial[i].id = 1 + i; s->Displacement[i].id = 11 + i; s->Filter[i].id = 21 + i; }
    s->RadialKeys.id = 10; s->DisplacementKeys.id = 20; s->FilterKeys.id = 30;
    s->Radial[0].push(1); s->Radial[2].push(2); s->Radial[2].push(3);
    s->Displacement[1].push(4);
    s->Filter[0].push(5); s->Filter[1].push(6); s->Filter[2].push(7); s->Filter[2].push(8);
    s->RadialKeys.insertChain(3); s->FilterKeys.insertChain(2);
    int allocs = g_allocs;
    delete s;
    if (g_entryDtors != 8) fail |= 1;
    if (g_frees != allocs) fail |= 2;                         // every node, tree node and sentinel freed
    static const int want[12] = { 30, 23, 22, 21, 20, 13, 12, 11, 10, 3, 2, 1 };   // reverse member order
    if (g_orderCount != 12) fail |= 4;
    for (int i = 0; i < 12 && i < g_orderCount; ++i) if (g_order[i] != want[i]) fail |= 8;
    if (fail) { std::printf("008814c0_TEST FAIL\n"); return 1; }
    std::printf("008814c0_TEST PASS\n");
    return 0;
}