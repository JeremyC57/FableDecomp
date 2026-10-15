// Behaviour test for 0x00537956 (NUISystem::CList::ChangePosition): transform the target in scrolling mode, then move and refresh.
#include <cstdio>
struct V { float x, y; };
struct Owner { long refs; bool scrolling; int deleted; };
struct List {
    Owner* owner; V moved; float time; bool immediate; int refresh[3]; int n;
    Owner* GetOwner() { ++owner->refs; return owner; }
    void Release(Owner* o) { if (--o->refs == 0) ++o->deleted; }
    V Transform(const V& p) { V r = { p.x * 2, p.y * 2 }; return r; }
    void BaseChangePosition(const V& p, float t, bool i) { moved = p; time = t; immediate = i; }
    void ChangePosition(const V& position, float t, bool i)
    {
        V target = position;
        Owner* o = GetOwner(); bool scrolling = o->scrolling; Release(o);
        if (scrolling) target = Transform(position);
        BaseChangePosition(target, t, i);
        refresh[n++] = 185; refresh[n++] = 187; refresh[n++] = 186;
    }
};
int main()
{
    Owner o = { 1, false, 0 };
    List l; l.owner = &o; l.n = 0;
    V p = { 3, 4 };
    l.ChangePosition(p, 0.5f, true);
    int ok = l.moved.x == 3 && l.moved.y == 4 && l.time == 0.5f && l.immediate && o.refs == 1 && o.deleted == 0 && l.refresh[1] == 187;
    o.scrolling = true; l.n = 0; l.ChangePosition(p, 1.0f, false);
    ok &= l.moved.x == 6 && l.moved.y == 8 && !l.immediate;
    if (!ok) { std::printf("00537956_TEST FAIL\n"); return 1; }
    std::printf("00537956_TEST PASS\n");
    return 0;
}
