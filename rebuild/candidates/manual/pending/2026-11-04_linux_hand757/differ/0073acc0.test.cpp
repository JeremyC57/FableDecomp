// Behaviour test for 0x0073ACC0 CTCCreatureModeManager::AddMode: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <list>
struct Mode { unsigned long type; bool replaced; int leave, resume; };
static bool AddMode(std::list<Mode*>& modes, Mode* m, unsigned long& removed)
{
    for (std::list<Mode*>::iterator it = modes.begin(); it != modes.end(); ++it) if ((*it)->type == m->type) return false;
    unsigned long replaced = 0;
    if (!modes.empty()) { if (modes.front()->replaced) replaced = modes.front()->type; modes.front()->leave++; }
    modes.push_back(m); modes.front()->resume++;
    if (replaced) removed = replaced;
    return true;
}
int main() {
    std::list<Mode*> l; Mode a = { 1, true, 0, 0 }, b = { 2, false, 0, 0 }, c = { 1, false, 0, 0 }; unsigned long r = 0;
    int ok = AddMode(l, &a, r) && a.resume == 1 && r == 0;
    ok &= AddMode(l, &b, r) && a.leave == 1 && r == 1 && a.resume == 2;
    ok &= !AddMode(l, &c, r) && l.size() == 2;
    std::printf(ok ? "0073acc0_TEST PASS\n" : "0073acc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
