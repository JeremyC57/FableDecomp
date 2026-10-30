// behaviour test for 0x00917E30 CAIStateGroup_Clean::GetBench()
#include <cstdio>
#include <list>
class CThing { public: bool isBench; bool occupied; bool hasOwner; std::list<CThing*> objects; };
class CIsFreeBench { public: bool operator()(CThing* t) const { return t && t->isBench && !t->occupied; } };
static CThing* GetBench(CThing* home)
{
    if (home) {
        if (home->hasOwner) {
            for (std::list<CThing*>::const_iterator it = home->objects.begin(); it != home->objects.end(); ++it)
                if (CIsFreeBench()(*it)) return *it;
        }
    }
    return 0;
}
int main()
{
    CThing used = { true, true, false }, chair = { false, false, false }, bench = { true, false, false };
    CThing home = { false, false, true };
    home.objects.push_back(&used); home.objects.push_back(&chair); home.objects.push_back(&bench);
    bool ok = GetBench(&home) == &bench && GetBench(0) == 0;
    CThing noOwner = home; noOwner.hasOwner = false;
    ok = ok && GetBench(&noOwner) == 0;
    home.objects.pop_back();
    ok = ok && GetBench(&home) == 0;
    std::printf(ok ? "00917e30_TEST PASS\n" : "00917e30_TEST FAIL\n");
    return ok ? 0 : 1;
}