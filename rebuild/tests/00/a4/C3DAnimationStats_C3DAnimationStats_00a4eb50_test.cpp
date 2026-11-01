// Behaviour test for 0x00A4EB50 C3DAnimationStats::~C3DAnimationStats: the TimingEvents tree is cleared only when it holds
// nodes (erase from the root, header relinked to itself, count zeroed), then the header node is freed, then ~CBaseObject.
#include <cstdio>
#include <cstdlib>
static int g_erased = 0, g_freed = 0, g_baseAt = -1;
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
static void Erase(NodeBase* x) { while (x) { Erase(x->right); NodeBase* l = x->left; std::free(x); ++g_erased; x = l; } }
struct Tree {
    NodeBase* header; unsigned count;
    void clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~Tree() { clear(); if (header) { std::free(header); ++g_freed; } }
};
class CBaseObject { public: virtual ~CBaseObject() { g_baseAt = g_freed; } void* IntelligentPointer; };
class C3DAnimationStats : public CBaseObject { public: virtual ~C3DAnimationStats() { } int AnimType; float Duration; Tree TimingEvents; };
static NodeBase* NewNode() { NodeBase* n = (NodeBase*)std::malloc(sizeof(NodeBase)); n->parent = n->left = n->right = 0; return n; }
int main()
{
    C3DAnimationStats* s = new C3DAnimationStats;
    s->TimingEvents.header = NewNode();
    NodeBase* root = NewNode(); root->left = NewNode(); root->right = NewNode();
    s->TimingEvents.header->parent = root; s->TimingEvents.count = 3;
    CBaseObject* b = s;
    delete b;
    if (g_erased != 3 || g_freed != 1 || g_baseAt != 1) { std::printf("00a4eb50_TEST FAIL\n"); return 1; }
    C3DAnimationStats* e = new C3DAnimationStats;            // empty tree: no erase, header still freed
    e->TimingEvents.header = NewNode(); e->TimingEvents.count = 0;
    delete e;
    if (g_erased != 3 || g_freed != 2) { std::printf("00a4eb50_TEST FAIL\n"); return 1; }
    std::printf("00a4eb50_TEST PASS\n");
    return 0;
}