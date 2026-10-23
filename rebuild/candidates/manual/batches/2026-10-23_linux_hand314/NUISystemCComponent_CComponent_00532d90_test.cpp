// behaviour test for 00532d90 NUISystem::CComponent::~CComponent (the detach steps over tiny stand-in objects)
#include <cstdio>
struct Comp;
struct Action { Comp* params[4]; int n; bool valid; };
struct Comp {
    Comp* parent; Comp* positionParent; Comp* positionChildren[4]; int npc; Comp* children[4]; int nc;
    Comp* toDelete[4]; int ntd; int deletionMethod; int deletedCalls; Action* actionsIAmParamOf[4]; int na;
    int removedPositionChild; int removedPositionParent;
    void Init() { parent = 0; positionParent = 0; npc = nc = ntd = na = 0; deletionMethod = 0; deletedCalls = 0; removedPositionChild = removedPositionParent = 0; }
    void Destroy()
    {
        for (int i = 0; i < na; ++i) {
            Action* a = actionsIAmParamOf[i];
            int w = 0;
            for (int j = 0; j < a->n; ++j) if (a->params[j] != this) a->params[w++] = a->params[j];
            a->n = w; a->valid = false;
        }
        if (positionParent) positionParent->removedPositionChild++;
        for (int i = 0; i < npc; ++i) positionChildren[i]->removedPositionParent++;
        for (int i = 0; i < nc; ++i) if (this == children[i]->parent) children[i]->parent = 0;
        for (int i = 0; i < ntd; ++i)
            if (this == toDelete[i]->parent) {
                toDelete[i]->parent = 0;
                if (toDelete[i]->deletionMethod == 0) toDelete[i]->deletedCalls++;
            }
    }
};
int main()
{
    Comp me, pp, c1, c2, d1, d2; me.Init(); pp.Init(); c1.Init(); c2.Init(); d1.Init(); d2.Init();
    Action act; act.params[0] = &me; act.params[1] = &pp; act.n = 2; act.valid = true;
    me.actionsIAmParamOf[0] = &act; me.na = 1;
    me.positionParent = &pp; me.positionChildren[0] = &c1; me.npc = 1;
    c1.parent = &me; c2.parent = &pp; me.children[0] = &c1; me.children[1] = &c2; me.nc = 2;
    d1.parent = &me; d2.parent = &me; d2.deletionMethod = 1; me.toDelete[0] = &d1; me.toDelete[1] = &d2; me.ntd = 2;
    me.Destroy();
    int ok = act.n == 1 && act.params[0] == &pp && !act.valid && pp.removedPositionChild == 1 && c1.removedPositionParent == 1
             && c1.parent == 0 && c2.parent == &pp && d1.parent == 0 && d1.deletedCalls == 1 && d2.parent == 0 && d2.deletedCalls == 0;
    std::printf(ok ? "00532d90_TEST PASS\n" : "00532d90_TEST FAIL\n");
    return ok ? 0 : 1;
}
