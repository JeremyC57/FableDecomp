// Behaviour test for 005bbb7d: map::insert(iterator hint, const value_type& x) -- forwards the hint (copied) and the value to the tree's insert_unique and returns its iterator
#include <cstdio>
#include <map>
struct Iter { void* node; Iter(void* n) : node(n) { } Iter(const Iter& o) : node(o.node) { } };
struct Tree {
    std::map<long, long> m; void* lastHint; const void* lastValue;
    Iter insert_unique(Iter hint, const std::pair<const long, long>& x) { lastHint = hint.node; lastValue = &x; m.insert(x); return Iter(&m[x.first]); }
};
struct Map { Tree t; Iter insert(Iter position, const std::pair<const long, long>& x) { return t.insert_unique(position, x); } };
int main()
{
    Map mp; mp.t.lastHint = 0; mp.t.lastValue = 0; int hintNode = 0;
    std::pair<const long, long> v(5, 50);
    Iter r = mp.insert(Iter(&hintNode), v);
    int ok = mp.t.lastHint == &hintNode && mp.t.lastValue == &v && r.node == &mp.t.m[5] && mp.t.m[5] == 50 && mp.t.m.size() == 1;
    std::printf(ok ? "005bbb7d_TEST PASS\n" : "005bbb7d_TEST FAIL\n");
    return ok ? 0 : 1;
}
