// Behaviour test for 0x004C4A20 _Rb_tree<void*, pair<void* const, CMeshEffectPriorityEntry>>::_M_create_node(v): a fresh
// node carries a copy of the key and entry; the counted mesh effect gains a reference (none for a null effect).
#include <cstdio>
#include <cstdlib>

struct Effect { void* vt; long RefCount; };
struct Entry { Effect* PEffect; long Priority; bool EffectsCarriedItems, EffectsWeapons, InheritedFromCarrier; };
struct Value { void* first; Entry second; };
struct Node { bool color; Node* parent; Node* left; Node* right; Value v; };

static Node* CreateNode(const Value& x)
{
    Node* tmp = static_cast<Node*>(std::malloc(sizeof(Node)));
    if (&tmp->v) {
        tmp->v.first = x.first;
        tmp->v.second.PEffect = x.second.PEffect;
        if (tmp->v.second.PEffect) ++tmp->v.second.PEffect->RefCount;
        tmp->v.second.Priority = x.second.Priority;
        tmp->v.second.EffectsCarriedItems = x.second.EffectsCarriedItems;
        tmp->v.second.EffectsWeapons = x.second.EffectsWeapons;
        tmp->v.second.InheritedFromCarrier = x.second.InheritedFromCarrier;
    }
    return tmp;
}

int main()
{
    Effect e = { 0, 1 };
    Value v = { (void*)0x1234, { &e, 3, true, false, true } };
    Node* n = CreateNode(v);
    if (n->v.first != (void*)0x1234 || n->v.second.PEffect != &e || e.RefCount != 2 || n->v.second.Priority != 3 ||
        !n->v.second.EffectsCarriedItems || n->v.second.EffectsWeapons || !n->v.second.InheritedFromCarrier) {
        std::printf("004c4a20_TEST FAIL\n"); return 1;
    }
    Value z = { 0, { 0, 0, false, true, false } };
    Node* m = CreateNode(z);
    if (m->v.second.PEffect != 0 || !m->v.second.EffectsWeapons || e.RefCount != 2) { std::printf("004c4a20_TEST FAIL\n"); return 1; }
    std::free(n); std::free(m);
    std::printf("004c4a20_TEST PASS\n");
    return 0;
}