// Behaviour test for 0x0065BE6C CTCVillage::UpdateHornTasks: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <list>
#include <algorithm>
struct Task { int horn; };
struct HasHorn { int h; HasHorn(int x) : h(x) {} bool operator()(const Task& t) const { return t.horn == h; } };
struct Village { std::list<int> horns; std::list<Task> tasks; bool added; int removed; };
static void Update(Village& v, bool curfew)
{
    if (curfew) {
        if (!v.added) {
            for (std::list<int>::iterator it = v.horns.begin(); it != v.horns.end(); ++it)
                if (std::find_if(v.tasks.begin(), v.tasks.end(), HasHorn(*it)) == v.tasks.end()) { Task t = { *it }; v.tasks.push_back(t); }
            v.added = true;
        }
    } else if (v.added) {
        for (std::list<int>::iterator it = v.horns.begin(); it != v.horns.end(); ++it) v.removed++;
        v.added = false;
    }
}
int main() {
    Village v; v.added = false; v.removed = 0; v.horns.push_back(1); v.horns.push_back(2); Task t = { 2 }; v.tasks.push_back(t);
    Update(v, true); int ok = v.tasks.size() == 2 && v.added;
    Update(v, true); ok &= v.tasks.size() == 2;
    Update(v, false); ok &= !v.added && v.removed == 2;
    std::printf(ok ? "0065be6c_TEST PASS\n" : "0065be6c_TEST FAIL\n");
    return ok ? 0 : 1;
}
