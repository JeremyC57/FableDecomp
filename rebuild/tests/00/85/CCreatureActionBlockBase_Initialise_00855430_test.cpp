// Behaviour test for 0x00855430 CCreatureAction_BlockBase::Initialise: play "HIT_BLOCKING" through component 0x10 when present, then base Initialise.
#include <cstdio>
#include <cstring>
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long First, Second; };
struct CSoundPlayParams {
    float Volume, Pitch, Delay; long Loop, Priority, Pan, Flags, Handle;
    CSoundPlayParams() { Volume = 1.0f; Pitch = 1.0f; Delay = 0.0f; Loop = 1; Pan = 0; Flags = 0; Handle = -1; Priority = 1; }
};
struct CTCSoundPlayer {
    int plays; long first; CSoundPlayParams p; long a;
    void PlaySound(const CSoundPair& s, const CSoundPlayParams& pp, long a_, long, long) { ++plays; first = s.First; p = pp; a = a_; }
};
struct CThingCreatureBase {
    bool has; CTCSoundPlayer* comp; const char* asked;
    template<class T> bool GetInterface(T** pp, long type) { if (type == 0x10 && has) { *pp = (T*)comp; return true; } return false; }
    CSoundPair GetSoundPair(const CCharString& n) { asked = n.t; CSoundPair s; s.First = 77; s.Second = 78; return s; }
};
struct CBaseIntelligentPointer { CThingCreatureBase* PItem; CThingCreatureBase* GetPItem() const { return PItem; } };
struct CCombatActionBase { CBaseIntelligentPointer Owner; int baseInit; void Initialise() { ++baseInit; } };
struct CCreatureAction_BlockBase : CCombatActionBase {
    void Initialise()
    {
        CTCSoundPlayer* player;
        if (Owner.GetPItem()->GetInterface(&player, 0x10))
            player->PlaySound(Owner.GetPItem()->GetSoundPair("HIT_BLOCKING"), CSoundPlayParams(), 1, 0, 0);
        CCombatActionBase::Initialise();
    }
};
int main()
{
    CTCSoundPlayer pl; pl.plays = 0;
    CThingCreatureBase c; c.has = true; c.comp = &pl; c.asked = 0;
    CCreatureAction_BlockBase b; b.Owner.PItem = &c; b.baseInit = 0;
    b.Initialise();
    bool ok = pl.plays == 1 && pl.first == 77 && c.asked && !std::strcmp(c.asked, "HIT_BLOCKING") && pl.p.Volume == 1.0f
        && pl.p.Handle == -1 && pl.p.Priority == 1 && pl.a == 1 && b.baseInit == 1;
    c.has = false; b.Initialise();
    ok = ok && pl.plays == 1 && b.baseInit == 2;
    if (!ok) { std::printf("00855430_TEST FAIL\n"); return 1; }
    std::printf("00855430_TEST PASS\n");
    return 0;
}