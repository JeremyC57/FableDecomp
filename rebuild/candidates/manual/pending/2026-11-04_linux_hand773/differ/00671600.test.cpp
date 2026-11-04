// Behaviour test for CThingCreatureDef::Transfer 0x00671600: a model persist context that records each named flag transfer;
// in text mode the navigator types are sorted and de-duplicated after loading, in binary mode they are kept as read; each
// bool bitfield goes through a whole-bool local (save: current value out; load: value in, bit written back, others kept).
#include <cstdio>
#include <algorithm>
struct Ctx {
    int Mode;                          // 2 / 3 binary, else text
    bool Loading; bool LoadValue;
    int Count;
    bool IsBinary() const { return Mode == 2 || Mode == 3; }
    void Transfer(const char*, bool& v, const bool& def) { ++Count; if (Loading) v = LoadValue; else if (!v) v = def && false; }
};
struct Def {
    int Nav[8]; int NavCount;
    bool LeaveDeadCreature : 1; bool AbleToStrafe : 1; bool CreateDummyOnDeath : 1; bool ImmuneToAttack : 1;
    bool UseMeleeWeaponAsDefault : 1; bool UnsheatheWeaponsAutomatically : 1; bool IsWillable : 1; bool IsIndestructible : 1;
    bool IsInvulnerableDuringGenericResponse : 1; bool IsPushedByExplosions : 1;
};
#define TRANSFER_FLAG(name, def) { bool flag = d.name; c.Transfer(#name, flag, def); d.name = flag; }
static void Transfer(Def& d, Ctx& c)
{
    if (!c.IsBinary()) {
        std::sort(d.Nav, d.Nav + d.NavCount);
        d.NavCount = (int)(std::unique(d.Nav, d.Nav + d.NavCount) - d.Nav);
    }
    TRANSFER_FLAG(LeaveDeadCreature, false) TRANSFER_FLAG(AbleToStrafe, false) TRANSFER_FLAG(CreateDummyOnDeath, false)
    TRANSFER_FLAG(ImmuneToAttack, false) TRANSFER_FLAG(UseMeleeWeaponAsDefault, true) TRANSFER_FLAG(UnsheatheWeaponsAutomatically, true)
    TRANSFER_FLAG(IsWillable, false) TRANSFER_FLAG(IsIndestructible, false) TRANSFER_FLAG(IsInvulnerableDuringGenericResponse, false)
    TRANSFER_FLAG(IsPushedByExplosions, true)
}
int main()
{
    int ok = 1;
    Def d; d.NavCount = 5; d.Nav[0] = 3; d.Nav[1] = 1; d.Nav[2] = 3; d.Nav[3] = 2; d.Nav[4] = 1;
    d.LeaveDeadCreature = d.AbleToStrafe = d.CreateDummyOnDeath = d.ImmuneToAttack = false;
    d.UseMeleeWeaponAsDefault = d.UnsheatheWeaponsAutomatically = d.IsWillable = false;
    d.IsIndestructible = false; d.IsInvulnerableDuringGenericResponse = false; d.IsPushedByExplosions = false;
    Ctx text = { 1, true, true, 0 };
    Transfer(d, text);
    if (d.NavCount != 3 || d.Nav[0] != 1 || d.Nav[1] != 2 || d.Nav[2] != 3) ok = 0;
    if (!d.IsIndestructible || !d.IsWillable || !d.IsInvulnerableDuringGenericResponse || text.Count != 10) ok = 0;
    Def b = d; b.NavCount = 2; b.Nav[0] = 5; b.Nav[1] = 5;
    Ctx bin = { 2, true, false, 0 };
    Transfer(b, bin);
    if (b.NavCount != 2 || b.IsIndestructible || b.IsWillable || !d.IsPushedByExplosions) ok = 0;
    std::printf(ok ? "00671600_TEST PASS\n" : "00671600_TEST FAIL\n");
    return ok ? 0 : 1;
}
