// Behaviour test for 0x00DDB570 NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Init: the data string picks
// DistanceToPlayAnim (STANDARD_POINT 12, CS_POINT 16, ST_COME_ON 10, AGGRESSIVE_POINTING 12, anything else 12), and the
// guard is made unkillable (EntitySetAsKillable(Me, false, true)).
#include <cstdio>
#include <cstring>

class CCharString { public:
    char Text[24];
    CCharString(const char* t) { std::strcpy(Text, t); }
    bool operator==(const char* t) const { return !std::strcmp(Text, t); }
};
class CScriptThing { public:
    char Data[24]; int Reads;
    CCharString GetDataString() const { ++const_cast<CScriptThing*>(this)->Reads; return CCharString(Data); }
};
class CGame { public:
    const CScriptThing* Thing; bool Killable; bool B; int Calls;
    void EntitySetAsKillable(const CScriptThing& t, bool killable, bool b) { Thing = &t; Killable = killable; B = b; ++Calls; }
};
struct CPrisonRaceGuard {
    CGame* Game; CScriptThing Me; long DistanceToPlayAnim;
    void Init()
    {
        DistanceToPlayAnim = 12;
        if (Me.GetDataString() == "STANDARD_POINT")
            DistanceToPlayAnim = 12;
        else if (Me.GetDataString() == "CS_POINT")
            DistanceToPlayAnim = 16;
        else if (Me.GetDataString() == "ST_COME_ON")
            DistanceToPlayAnim = 10;
        else if (Me.GetDataString() == "AGGRESSIVE_POINTING")
            DistanceToPlayAnim = 12;
        Game->EntitySetAsKillable(Me, false, true);
    }
};

static int Run(const char* data, long want, int reads)
{
    CGame g; g.Calls = 0; g.Killable = true; g.B = false;
    CPrisonRaceGuard p; p.Game = &g; std::strcpy(p.Me.Data, data); p.Me.Reads = 0; p.DistanceToPlayAnim = -5;
    p.Init();
    return p.DistanceToPlayAnim == want && p.Me.Reads == reads && g.Calls == 1 && g.Thing == &p.Me && !g.Killable && g.B;
}

int main()
{
    int ok = Run("STANDARD_POINT", 12, 1) && Run("CS_POINT", 16, 2) && Run("ST_COME_ON", 10, 3)
          && Run("AGGRESSIVE_POINTING", 12, 4) && Run("OTHER", 12, 4);
    if (!ok) { std::printf("00ddb570_TEST FAIL\n"); return 1; }
    std::printf("00ddb570_TEST PASS\n");
    return 0;
}