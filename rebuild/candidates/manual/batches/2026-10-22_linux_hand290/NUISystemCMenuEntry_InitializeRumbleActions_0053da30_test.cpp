// Behaviour test for NUISystem::CMenuEntry::InitializeRumbleActions (0x0053DA30): copy the repeat setting and add one action
// per non-zero rumble id to the matching list; the def reference is released at the end.
#include <cstdio>

struct Def { int refCount; int selected, back, pre, visible, unselected; int repeat; bool destroyed; };
struct Action { int type; };
static int g_live = 0;
struct ActionPtr {
    Action* p; int* count;
    explicit ActionPtr(Action* a) : p(a), count(new int(1)) { ++g_live; }
    ActionPtr(const ActionPtr& o) : p(o.p), count(o.count) { ++*count; }
    ~ActionPtr() { if (--*count == 0) { delete p; delete count; --g_live; } }
    Action* operator->() const { return p; }
};
struct DefPtr {
    Def* d;
    DefPtr(Def* x) : d(x) { ++d->refCount; }
    DefPtr(const DefPtr& o) : d(o.d) { ++d->refCount; }
    ~DefPtr() { if (d && --d->refCount == 0) d->destroyed = true; }
    Def* operator->() const { return d; }
};
struct Entry {
    Def* def;
    int repeat;
    int got[5]; int gotCount[5];
    DefPtr GetDef() { return DefPtr(def); }
    void Add(int slot, const ActionPtr& a) { got[slot] = a->type; ++gotCount[slot]; }
    void Init()
    {
        DefPtr d = GetDef();
        repeat = d->repeat;
        if (d->selected) { ActionPtr a(new Action); a->type = d->selected; Add(0, a); }
        if (d->back) { ActionPtr a(new Action); a->type = d->back; Add(1, a); }
        if (d->pre) { ActionPtr a(new Action); a->type = d->pre; Add(2, a); }
        if (d->visible) { ActionPtr a(new Action); a->type = d->visible; Add(3, a); }
        if (d->unselected) { ActionPtr a(new Action); a->type = d->unselected; Add(4, a); }
    }
};

int main()
{
    Def def; def.refCount = 0; def.selected = 7; def.back = 0; def.pre = 9; def.visible = 0; def.unselected = 11; def.repeat = 3; def.destroyed = false;
    Entry e; e.def = &def;
    for (int i = 0; i < 5; ++i) { e.got[i] = 0; e.gotCount[i] = 0; }
    e.Init();
    if (e.repeat != 3 || e.got[0] != 7 || e.gotCount[1] != 0 || e.got[2] != 9 || e.gotCount[3] != 0 || e.got[4] != 11
        || g_live != 0 || !def.destroyed) {
        std::printf("0053da30_TEST FAIL\n"); return 1;
    }
    std::printf("0053da30_TEST PASS\n");
    return 0;
}
