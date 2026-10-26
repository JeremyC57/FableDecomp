// Behaviour test for CTCBase::GetThingManager() (0x00686AE0): returns the global thing manager by reference.
#include <cstdio>

class CThingManager { public: int Id; };
CThingManager* g_pThingManager;

class CTCBase {
protected:
    CThingManager& GetThingManager();
public:
    CThingManager& Probe() { return GetThingManager(); }
};

CThingManager& CTCBase::GetThingManager()
{
    return *g_pThingManager;
}

int main()
{
    static CThingManager mgr;
    mgr.Id = 41;
    g_pThingManager = &mgr;
    CTCBase tc;
    int ok = (&tc.Probe() == &mgr) && tc.Probe().Id == 41;
    static CThingManager other;
    g_pThingManager = &other;
    ok &= (&tc.Probe() == &other);
    if (ok) printf("PASS_00686ae0\n");
    return ok ? 0 : 1;
}
