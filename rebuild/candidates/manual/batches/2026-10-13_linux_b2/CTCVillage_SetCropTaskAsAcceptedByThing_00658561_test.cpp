#include <cstdio>

struct CThing { int x; };
static CThing* g_by = 0;
struct Task { bool avail; virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){} virtual bool IsAvailable() { return avail; }
    virtual void v14(){} virtual void v18(){} virtual void v1c(){} virtual void SetAcceptedBy(CThing* t) { g_by = t; } };
struct V { bool Set(Task* t, CThing* th) { if (!t->IsAvailable()) return false; t->SetAcceptedBy(th); return true; } };
int main()
{
    Task t; CThing th; V v; t.avail = false; if (v.Set(&t, &th) || g_by) { std::printf("00658561_TEST FAIL\n"); return 1; }
    t.avail = true; if (!v.Set(&t, &th) || g_by != &th) { std::printf("00658561_TEST FAIL\n"); return 1; }
    std::printf("00658561_TEST PASS\n");
    return 0;
}
