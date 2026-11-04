// Behaviour test for 0x007729A0 CTCShop::CalcIsStallLike: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <list>
struct T { bool dead; bool tc26; };
static bool StallLike(bool permanent, bool hasList, std::list<T*>& things)
{
    if (permanent) return false;
    if (hasList) for (std::list<T*>::iterator it = things.begin(); it != things.end(); ++it) if (*it && !(*it)->dead && (*it)->tc26) return false;
    return true;
}
int main() {
    T a = { true, true }, b = { false, false }, c = { false, true }; std::list<T*> l; l.push_back(0); l.push_back(&a); l.push_back(&b);
    int ok = StallLike(false, true, l) && !StallLike(true, true, l);
    l.push_back(&c); ok &= !StallLike(false, true, l) && StallLike(false, false, l);
    std::printf(ok ? "007729a0_TEST PASS\n" : "007729a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
