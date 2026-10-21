#include <cstdio>
// Behaviour copy of CCreatureAction_JackScream::Initialise (0x008C0D10): counted-pointer assignment with release of the old block, mode replacement.
struct Block { long refs; int* object; };
static int destroyed = 0;
struct Counted {
    int* obj; Block* count;
    Counted() : obj(0), count(0) {}
    explicit Counted(int* o) : obj(o), count(o ? new Block : 0) { if (count) { count->refs = 1; count->object = o; } }
    Counted(const Counted& o) : obj(o.obj), count(o.count) { if (count) ++count->refs; }
    ~Counted() { if (count && --count->refs == 0) { ++destroyed; delete count; } }
    Counted& operator=(const Counted& o) {
        Block* c = o.count; int* ob = o.obj;
        if (count != c) { if (count) { --count->refs; if (count->refs == 0) { ++destroyed; delete count; } } obj = ob; count = c; if (c) ++c->refs; }
        return *this;
    }
};
struct Manager { bool has31; bool removed; int added; void AddMode(Counted m) { ++added; } };
struct Action {
    Manager* mgr; Counted PMode; int sword; int* swordSet;
    void Initialise() {
        if (mgr->has31) mgr->removed = true;
        PMode = Counted(new int(5));
        *PMode.obj = sword;                      // SetSwordObject
        mgr->AddMode(Counted(PMode));
    }
};
int main()
{
    Manager m = { true, false, 0 };
    Action a; a.mgr = &m; a.sword = 42;
    int* old = new int(1); a.PMode = Counted(old);
    bool ok = a.PMode.count->refs == 1;
    int* oldObj = a.PMode.obj;
    a.Initialise();
    ok = ok && m.removed && m.added == 1 && a.PMode.obj != oldObj && *a.PMode.obj == 42 && a.PMode.count->refs == 1 && destroyed == 1;
    delete old; delete a.PMode.obj;
    std::printf(ok ? "008c0d10_TEST PASS\n" : "008c0d10_TEST FAIL\n");
    return ok ? 0 : 1;
}