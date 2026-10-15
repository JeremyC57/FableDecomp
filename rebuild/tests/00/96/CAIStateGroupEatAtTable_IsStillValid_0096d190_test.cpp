// Behaviour test for 0x0096D190: remove mode 0x28; invalid if it persists, else defer to the virtual check.
#include <cstdio>
struct CModes { int removed; bool sticky; bool has;
    void RemoveMode(unsigned long m) { if (m == 0x28) { ++removed; if (!sticky) has = false; } }
    bool HasMode(unsigned long m) { return m == 0x28 && has; } };
struct CGroup {
    CModes* modes; bool deferred;
    virtual bool IsStateGroupStillValid() { return deferred; }
    bool IsStillValid()
    {
        CModes* m = modes;
        m->RemoveMode(0x28);
        if (m->HasMode(0x28)) return false;
        return IsStateGroupStillValid();
    }
    virtual ~CGroup() {}
};
int main()
{
    CModes m = { 0, false, true };
    CGroup g; g.modes = &m; g.deferred = true;
    int bad = 0;
    if (!g.IsStillValid() || m.removed != 1) bad = 1;
    g.deferred = false; if (g.IsStillValid()) bad = 2;
    m.sticky = true; m.has = true; g.deferred = true; if (g.IsStillValid()) bad = 3;
    if (bad) { std::printf("0096d190_TEST FAIL\n"); return 1; }
    std::printf("0096d190_TEST PASS\n");
    return 0;
}