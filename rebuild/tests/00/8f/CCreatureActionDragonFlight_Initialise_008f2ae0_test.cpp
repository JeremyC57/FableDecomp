#include <stdio.h>

// ---- standalone copies of the extern deps for the test link ----
float g_dragonFlightScale = 2.5f;

struct CCreatureAction_Base
{
    virtual void Initialise();
};

class CCreatureAction_DragonFlight : public CCreatureAction_Base
{ public:
    char pad4[0x10 - 4];
    int  TotalTime;
    int  ActionTime;
    virtual void Initialise();
};

static int g_baseCalled = 0;
void __fastcall CCreatureAction_Base_Initialise(CCreatureAction_Base* action)
{
    (void)action;
    g_baseCalled = 1;
}

void CCreatureAction_Base::Initialise() {}

void CCreatureAction_DragonFlight::Initialise()
{
    CCreatureAction_Base_Initialise(this);
    this->ActionTime = (int)((float)this->TotalTime * g_dragonFlightScale);
}

int main()
{
    CCreatureAction_DragonFlight a;
    a.TotalTime = 10;
    a.ActionTime = -1;
    g_baseCalled = 0;

    a.Initialise();

    // 10 * 2.5 = 25.0 -> (int)25
    int ok = (g_baseCalled == 1) && (a.ActionTime == 25);

    // negative truncation check: -3 * 2.5 = -7.5 -> ftol truncates toward zero -> -7
    a.TotalTime = -3;
    a.Initialise();
    ok = ok && (a.ActionTime == -7);

    printf(ok ? "DRAGONFLIGHT_INIT_OK\n" : "FAIL\n");
    return ok ? 0 : 1;
}