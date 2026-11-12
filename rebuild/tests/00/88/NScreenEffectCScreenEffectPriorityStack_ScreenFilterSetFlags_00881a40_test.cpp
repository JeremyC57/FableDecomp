#include <cstdio>
#include <map>
#include <set>
#include <vector>
// Behaviour model of ScreenFilterSetFlags 0x00881A40.
struct Graphic { signed char ByPassColorFilter; void SetByPassColorFilter(bool b) { ByPassColorFilter = b ? 1 : 0; }
                 bool GetByPassColorFilter() const { return ByPassColorFilter > 0; } };
struct Thing { bool dead; Graphic* g; };
struct Arg { Thing* thing; bool bypassSet; };
struct State { Thing* thing; bool bypassReset; State() : thing(0), bypassReset(false) {}
    void Reset() { if (thing != 0) { if (!thing->dead) { Graphic* g = thing->g; if (g) g->SetByPassColorFilter(bypassReset); } } } };
struct Entry { std::map<Thing*, State> reset; };
struct Stack { std::set<Entry*> keys;
    void ScreenFilterSetFlags(const std::vector<Arg>& things, void* handle) {
        std::set<Entry*>::iterator found = keys.find((Entry*)handle);
        if (found == keys.end()) return;
        Entry* e = (Entry*)handle;
        for (int i = 0; i < (int)things.size(); ++i) {
            std::map<Thing*, State>::iterator it = e->reset.find(things[i].thing);
            if (it != e->reset.end()) { it->second.Reset(); e->reset.erase(it); }
        }
        for (int j = 0; j < (int)things.size(); ++j) {
            Thing* t = things[j].thing;
            if (t != 0 && !t->dead) {
                Graphic* g = t->g;
                if (g) { State s; s.thing = t; s.bypassReset = g->GetByPassColorFilter();
                         e->reset[s.thing] = s; g->SetByPassColorFilter(things[j].bypassSet); }
            }
        }
    }
};
int main()
{
    Graphic ga = {1}, gb = {0};
    Thing a = {false, &ga}, b = {false, &gb}, dead = {true, &gb};
    Entry entry; Stack stack;
    std::vector<Arg> args; Arg x = {&a, false}; Arg y = {&b, true}; Arg z = {&dead, true};
    args.push_back(x); args.push_back(y); args.push_back(z);
    stack.ScreenFilterSetFlags(args, &entry);              // not on the stack: nothing happens
    if (!entry.reset.empty() || ga.ByPassColorFilter != 1) { std::printf("00881a40_TEST FAIL\n"); return 1; }
    stack.keys.insert(&entry);
    stack.ScreenFilterSetFlags(args, &entry);
    if (entry.reset.size() != 2 || ga.ByPassColorFilter != 0 || gb.ByPassColorFilter != 1 ||
        entry.reset[&a].bypassReset != true || entry.reset[&b].bypassReset != false) { std::printf("00881a40_TEST FAIL\n"); return 1; }
    stack.ScreenFilterSetFlags(args, &entry);              // restore first, then re-save the applied state
    if (entry.reset.size() != 2 || entry.reset[&a].bypassReset != true || ga.ByPassColorFilter != 0 ||
        entry.reset[&b].bypassReset != false || gb.ByPassColorFilter != 1) { std::printf("00881a40_TEST FAIL\n"); return 1; }
    std::vector<Arg> one; Arg w = {&a, true}; one.push_back(w); a.dead = true;
    stack.ScreenFilterSetFlags(one, &entry);               // dead thing: entry forgotten, nothing applied
    if (entry.reset.size() != 1 || entry.reset.count(&a) != 0 || ga.ByPassColorFilter != 0) { std::printf("00881a40_TEST FAIL\n"); return 1; }
    std::printf("00881a40_TEST PASS\n");
    return 0;
}