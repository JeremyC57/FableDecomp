// behaviour test for NUISystem::CKeyRedefiner::Initialise 0x00558770: after the base initialise, each list (header, action)
// is created only when its pointer is unset and the def names one; it is appended to the children, initialised, parented
// to the redefiner and kept as the list pointer
#include <cstdio>
#include <vector>
struct Comp { int def; bool init; void* parent; };
struct Redefiner {
    bool baseInit; std::vector<Comp*> children; Comp* header; Comp* actions;
    void Initialise(int headerDef, int actionDef)
    {
        baseInit = true;
        if (!header && headerDef) {
            children.push_back(new Comp()); Comp* c = children[children.size() - 1];
            c->def = headerDef; c->init = true; c->parent = this; header = c;
        }
        if (!actions && actionDef) {
            children.push_back(new Comp()); Comp* c = children[children.size() - 1];
            c->def = actionDef; c->init = true; c->parent = this; actions = c;
        }
    }
};
int main()
{
    Redefiner a = { false, std::vector<Comp*>(), 0, 0 };
    a.Initialise(3, 0);
    bool ok = a.baseInit && a.children.size() == 1 && a.header && a.header->def == 3 && a.header->init && a.header->parent == &a && !a.actions;
    a.Initialise(4, 5);
    ok = ok && a.children.size() == 2 && a.header->def == 3 && a.actions && a.actions->def == 5 && a.actions->parent == &a;
    a.Initialise(6, 7);
    ok = ok && a.children.size() == 2;
    for (size_t i = 0; i < a.children.size(); ++i) delete a.children[i];
    std::printf(ok ? "00558770_TEST PASS\n" : "00558770_TEST FAIL\n");
    return ok ? 0 : 1;
}