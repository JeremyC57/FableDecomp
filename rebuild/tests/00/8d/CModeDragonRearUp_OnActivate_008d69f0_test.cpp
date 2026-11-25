#include <cstdio>

struct CModeDragonRearUp;

struct SomeState {
    char pad[0x10];
    unsigned char flag;
};

struct ModeBase {
    virtual ~ModeBase();
};

struct CModeDragonRearUp : public ModeBase {
    SomeState* state;
    void OnActivate();
};

static int g_baseCalls = 0;
static int g_doCalls = 0;
static int g_lastArg = -1;
static CModeDragonRearUp* g_lastSelf = 0;

ModeBase::~ModeBase() {}
// Naming (NF443, 2026-11-25; NF438 q2): the callee stub is renamed with the row to its PDB spelling ?OnActivate@CModeStandBase@@UAEXXZ (was ?BaseOnActivate@ModeBase@@QAEXXZ).
class CModeStandBase { public: virtual void OnActivate(); };
void CModeStandBase::OnActivate() { g_baseCalls++; }
class CThingAICreature { public: void SuspendAI(bool suspend, void* suspender); };   // 0x00833A10 (Ego_r.pdb)
void CThingAICreature::SuspendAI(bool suspend, void* suspender) { g_doCalls++; g_lastArg = suspend; g_lastSelf = (CModeDragonRearUp*)suspender; }

int main()
{
    SomeState st1; st1.flag = 1;
    CModeDragonRearUp m1; m1.state = &st1;
    m1.OnActivate();
    if (g_baseCalls != 1) { printf("FAIL base\n"); return 1; }
    if (g_doCalls != 1 || g_lastArg != 1 || g_lastSelf != &m1) { printf("FAIL do\n"); return 1; }

    SomeState st2; st2.flag = 0;
    CModeDragonRearUp m2; m2.state = &st2;
    m2.OnActivate();
    if (g_baseCalls != 2) { printf("FAIL base2\n"); return 1; }
    if (g_doCalls != 1) { printf("FAIL do2\n"); return 1; }

    printf("REARUP_OK\n");
    return 0;
}