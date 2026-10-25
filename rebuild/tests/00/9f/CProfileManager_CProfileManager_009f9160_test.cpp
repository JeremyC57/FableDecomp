// Behaviour test for 0x009F9160 CProfileManager::CProfileManager (empty containers, singleton, render manager, root tree entry).
#include <cstdio>
#include <cstdlib>

struct ListNode { ListNode* next; ListNode* prev; };
struct TreeNode { char color; TreeNode* parent; TreeNode* left; TreeNode* right; long key; long value; };
struct Entry { char profile[0x48]; long id; void* children[3]; Entry* parent; };
struct EntryNode { ListNode link; Entry data; };

struct RenderManager { int dummy; };
struct DisplayManager { int pad; void* sys; RenderManager* render; };
struct SystemManager { DisplayManager* display; };

static SystemManager g_system;
static int g_calls = 0;
static SystemManager* GetSystemManager() { ++g_calls; return &g_system; }

struct Manager;
static Manager* g_manager = 0;

static ListNode* NewListHead() { ListNode* n = (ListNode*)std::malloc(sizeof(EntryNode)); n->next = n; n->prev = n; return n; }
static TreeNode* NewTreeHead()
{
    TreeNode* h = (TreeNode*)std::malloc(sizeof(TreeNode));
    h->color = 0; h->parent = 0; h->left = h; h->right = h;
    return h;
}

struct Manager {
    bool up, down, toggle; void* cursor; bool minUpdated; double minTotal; long noTurns;
    RenderManager* render; int drawMode; int evalMode;
    ListNode* profileList; TreeNode* idToName; unsigned int idCount; TreeNode* nameToId; unsigned int nameCount;
    ListNode* tree; Entry* current; bool needsSaving;
    Manager() : up(false), down(false), toggle(false), cursor(0), render(0), needsSaving(false)
    {
        profileList = NewListHead();
        idToName = NewTreeHead(); idCount = 0;
        nameToId = NewTreeHead(); nameCount = 0;
        tree = NewListHead();
        if (g_manager == 0) g_manager = this;
        if (GetSystemManager()->display != 0) render = GetSystemManager()->display->render;
        EntryNode* e = (EntryNode*)std::malloc(sizeof(EntryNode));       // insert(end()): before the header
        e->data.children[0] = e->data.children[1] = e->data.children[2] = 0;
        e->link.next = tree; e->link.prev = tree->prev; tree->prev->next = &e->link; tree->prev = &e->link;
        ((EntryNode*)tree->next)->data.parent = 0;
        current = &((EntryNode*)tree->prev)->data;
        minTotal = 0.0; noTurns = 0; drawMode = 0; minUpdated = false; evalMode = 1;
    }
};

int main()
{
    int ok = 1;
    RenderManager rm;
    DisplayManager dm; dm.render = &rm;
    g_system.display = 0;
    Manager a;
    if (g_manager != &a || a.render != 0 || g_calls != 1) ok = 0;
    if (a.profileList->next != a.profileList || a.idToName->left != a.idToName || a.idToName->parent != 0 ||
        a.idCount != 0 || a.nameToId->right != a.nameToId || a.nameCount != 0) ok = 0;
    if (a.tree->next == a.tree || a.tree->next != a.tree->prev || a.current != &((EntryNode*)a.tree->next)->data ||
        a.current->parent != 0) ok = 0;
    if (a.minTotal > 1e-9 || a.minTotal < -1e-9 || a.noTurns != 0 || a.drawMode != 0 || a.evalMode != 1 || a.minUpdated ||
        a.up || a.down || a.toggle || a.cursor != 0 || a.needsSaving) ok = 0;
    g_system.display = &dm;
    Manager b;
    if (g_manager != &a || b.render != &rm || g_calls != 3) ok = 0;
    if (!ok) { std::printf("009f9160_TEST FAIL\n"); return 1; }
    std::printf("009f9160_TEST PASS\n");
    return 0;
}