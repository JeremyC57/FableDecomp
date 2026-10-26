// Behaviour test for 0x009F9020 CProfileManager::~CProfileManager (clear tree and maps, free every node and sentinel).
#include <cstdio>
#include <cstdlib>

static int g_freed = 0;
static int g_erased = 0;
static void Free(void* p) { if (p) ++g_freed; std::free(p); }

struct ListNode { ListNode* next; ListNode* prev; };
class TreeNode { public: char color; TreeNode* parent; TreeNode* left; TreeNode* right; };

static ListNode* NewHead() { ListNode* n = (ListNode*)std::malloc(sizeof(ListNode)); n->next = n->prev = n; return n; }
static void PushBack(ListNode* h) { ListNode* n = (ListNode*)std::malloc(sizeof(ListNode)); n->next = h; n->prev = h->prev; h->prev->next = n; h->prev = n; }
static void ClearList(ListNode* h)
{
    ListNode* cur = h->next;
    while (cur != h) { ListNode* tmp = cur; cur = cur->next; Free(tmp); }
    h->next = h->prev = h;
}
static void Erase(TreeNode* x) { while (x) { Erase(x->right); TreeNode* l = x->left; Free(x); ++g_erased; x = l; } }

struct Map {
    TreeNode* header; unsigned int count;
    Map() { header = (TreeNode*)std::malloc(sizeof(TreeNode)); header->color = 0; header->parent = 0; header->left = header->right = header; count = 0; }
    void Insert1() { TreeNode* n = (TreeNode*)std::malloc(sizeof(TreeNode)); n->parent = header; n->left = n->right = 0; header->parent = n; header->left = header->right = n; count = 1; }
    void Clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~Map() { Clear(); if (header) Free(header); }
};
struct List {
    ListNode* head;
    List() { head = NewHead(); }
    ~List() { ClearList(head); if (head) Free(head); }
};

struct Manager {
    List profileList; Map idToName; Map nameToId; List tree;
    ~Manager()
    {
        int walked = 0;
        for (ListNode* it = tree.head->next; it != tree.head; it = it->next) ++walked;
        ClearList(tree.head);
        idToName.Clear();
        nameToId.Clear();
    }
};

int main()
{
    int ok = 1;
    {
        Manager m;
        PushBack(m.profileList.head); PushBack(m.profileList.head);
        PushBack(m.tree.head);
        m.idToName.Insert1(); m.nameToId.Insert1();
    }
    if (g_freed != 3 + 2 + 4 || g_erased != 2) ok = 0;    // 3 list nodes, 2 tree nodes, 4 sentinels
    if (!ok) { std::printf("009f9020_TEST FAIL\n"); return 1; }
    std::printf("009f9020_TEST PASS\n");
    return 0;
}