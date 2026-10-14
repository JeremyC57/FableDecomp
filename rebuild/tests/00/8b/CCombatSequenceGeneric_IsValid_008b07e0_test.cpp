#include <cstdio>
struct CThingCreatureBase { int x; };
typedef bool (__fastcall* FnModule)(CThingCreatureBase*, CThingCreatureBase*);
static int g_calls = 0; static CThingCreatureBase* g_c = 0; static CThingCreatureBase* g_t = 0;
static bool __fastcall Module(CThingCreatureBase* c, CThingCreatureBase* t) { ++g_calls; g_c = c; g_t = t; return false; }
struct CTree {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; FnModule value; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head; unsigned long size;
    iterator find(const long& key) { node* n = head->parent; while (n) { if (n->key == key) return iterator(n); n = n->key < key ? n->right : n->left; } return iterator(head); }
};
static CTree g_tree;
struct CDef { long key; };
struct CSeq {
    CThingCreatureBase* Creature; CThingCreatureBase* Target; const CDef* Def_Object;
    bool IsValid() { const CDef* def = Def_Object; if (def->key != 0) { CTree::iterator it = g_tree.find(def->key); if (it.current != g_tree.head && it.current->value) return it.current->value(Creature, Target); } return true; }
};
int main()
{
    CTree::node head = { 0, { 0, 0, 0 }, 0, 0, 0, 0, 0 }; CTree::node n1 = { 0, { 0, 0, 0 }, &head, 0, 0, 3, Module }; CTree::node n2 = { 0, { 0, 0, 0 }, &n1, 0, 0, 5, 0 };
    n1.right = &n2; head.parent = &n1; g_tree.head = &head; g_tree.size = 2;
    CThingCreatureBase c, t; CDef d = { 3 }; CSeq s; s.Creature = &c; s.Target = &t; s.Def_Object = &d;
    if (s.IsValid()) { std::printf("008b07e0_TEST FAIL\n"); return 1; }
    if (g_calls != 1 || g_c != &c || g_t != &t) { std::printf("008b07e0_TEST FAIL\n"); return 1; }
    d.key = 5; if (!s.IsValid()) { std::printf("008b07e0_TEST FAIL\n"); return 1; } if (g_calls != 1) { std::printf("008b07e0_TEST FAIL\n"); return 1; }
    d.key = 9; if (!s.IsValid()) { std::printf("008b07e0_TEST FAIL\n"); return 1; } if (g_calls != 1) { std::printf("008b07e0_TEST FAIL\n"); return 1; }
    d.key = 0; if (!s.IsValid()) { std::printf("008b07e0_TEST FAIL\n"); return 1; } if (g_calls != 1) { std::printf("008b07e0_TEST FAIL\n"); return 1; }
    std::printf("008b07e0_TEST PASS\n");
    return 0;
}