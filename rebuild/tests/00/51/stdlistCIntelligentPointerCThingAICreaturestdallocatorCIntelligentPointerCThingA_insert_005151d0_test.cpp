// Behaviour test for 0x005151D0 std::list<CIntelligentPointer<CThingAICreature> >::insert(position, x): a new node
// holding a copy of x (same item) is linked before position and returned.
#include <cstdio>
#include <cstdlib>
#include <new>

struct Item { int id; };

struct BasePointerModel {
    Item* PItem;
    BasePointerModel() : PItem(0) { }
    Item* GetPItem() const { return PItem; }
    void SetPItem(Item* i) { PItem = i; }
};
struct CreaturePointerModel : BasePointerModel {
    CreaturePointerModel(Item* i) { SetPItem(i); }
    CreaturePointerModel(const CreaturePointerModel& o) : BasePointerModel() { SetPItem(o.GetPItem()); }
};

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct PtrNode : NodeBase { CreaturePointerModel _M_data; };

static NodeBase* insert_model(NodeBase* n, const CreaturePointerModel& x)
{
    PtrNode* tmp = (PtrNode*)std::malloc(sizeof(PtrNode));
    new (&tmp->_M_data) CreaturePointerModel(x);
    NodeBase* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
    return tmp;
}

int main()
{
    NodeBase* s = (NodeBase*)std::malloc(sizeof(PtrNode));
    s->_M_next = s->_M_prev = s;
    Item a = { 1 }, b = { 2 };
    NodeBase* na = insert_model(s, CreaturePointerModel(&a));
    NodeBase* nb = insert_model(na, CreaturePointerModel(&b));   // before na
    if (s->_M_next != nb || nb->_M_next != na || na->_M_next != s || s->_M_prev != na || na->_M_prev != nb) { std::printf("005151d0_TEST FAIL\n"); return 1; }
    if (((PtrNode*)na)->_M_data.GetPItem() != &a || ((PtrNode*)nb)->_M_data.GetPItem() != &b) { std::printf("005151d0_TEST FAIL\n"); return 1; }
    std::printf("005151d0_TEST PASS\n");
    return 0;
}