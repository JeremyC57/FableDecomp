// Test for 0x006869D0 CTCBase::GetDefinitionManager: the component's definition manager is its parent thing's.
#include <stdio.h>
class CGameDefinitionManager { public: int tag; };
static CGameDefinitionManager g_mgr;
static int g_calls = 0;
class CThing {
public:
    int id;
    const CGameDefinitionManager& PeekDefinitionManager() const;
};
const CThing* g_lastThing = 0;
const CGameDefinitionManager& CThing::PeekDefinitionManager() const { ++g_calls; g_lastThing = this; return g_mgr; }
class CTCBase {
public:
    virtual ~CTCBase() { }
    CTCBase(CThing* t) : ParentThing(t), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
protected:
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    const CGameDefinitionManager& GetDefinitionManager() const;
};
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const
{
    return ParentThing->PeekDefinitionManager();
}
class CTCProbe : public CTCBase {
public:
    CTCProbe(CThing* t) : CTCBase(t) { }
    const CGameDefinitionManager* Get() const { return &GetDefinitionManager(); }
};
int main()
{
    CThing a; a.id = 1; CThing b; b.id = 2;
    CTCProbe pa(&a), pb(&b);
    bool ok = pa.Get() == &g_mgr && g_lastThing == &a && pb.Get() == &g_mgr && g_lastThing == &b && g_calls == 2;
    if (!ok) { puts("006869d0_TEST FAIL"); return 1; }
    puts("006869d0_TEST PASS");
    return 0;
}
