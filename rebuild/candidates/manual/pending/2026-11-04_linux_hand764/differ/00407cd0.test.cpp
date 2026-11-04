// Behaviour test for 0x00407CD0 CUserProfileManager::RedefineControlPassive(const CActionInputControl&, const CActionInputControl&, long): a model; checks every matching entry is replaced except for the two context-sensitive actions, where only the index-th occurrence is.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct E { int action; int control; };
static void Redefine(std::vector<E>& t, int newC, int oldC, int oldAction, long index)
{
    long c37 = 0, c38 = 0;
    for (size_t i = 0; i < t.size(); ++i) {
        bool same = t[i].control == oldC;
        if (oldAction == 37) { if (c37 == index && same) t[i].control = newC; }
        else if (oldAction == 38) { if (c38 == index && same) t[i].control = newC; }
        else if (same) t[i].control = newC;
        if (t[i].action == 37) ++c37; else if (t[i].action == 38) ++c38;
    }
}
int main()
{
    int ok = 1; std::vector<E> t; E a = { 1, 5 }, b = { 37, 5 }, c = { 37, 5 };
    t.push_back(a); t.push_back(a); Redefine(t, 9, 5, 1, 0); ok &= t[0].control == 9 && t[1].control == 9;
    t.clear(); t.push_back(b); t.push_back(c); Redefine(t, 9, 5, 37, 1); ok &= t[0].control == 5 && t[1].control == 9;
    std::printf(ok ? "00407CD0_TEST PASS\n" : "00407CD0_TEST FAIL\n"); return ok ? 0 : 1;
}
