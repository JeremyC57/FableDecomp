// Behaviour test for 0x0000953DF0 CCreatureAction_DragonBurninateInto copy ctor.
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CSoundPair { public: long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
static const char* g_sound = 0;
CSoundPair GetSoundPair(CThingCreatureBase*, const CCharString& n) { g_sound = n.t; CSoundPair s; s.FirstSound = 9; return s; }
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CVectorC3DVector { int n; CVectorC3DVector(int k) : n(k) {} CVectorC3DVector(const CVectorC3DVector& o) : n(o.n) {} };
class CCombatActionBase { public:
    CThingCreatureBase* owner; long code; long s0;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair& s) : owner(o), code(c), s0(s.FirstSound) {}
};
class CCreatureAction_DragonBurninateInto : public CCombatActionBase { public:
    C3DVector PreviousPoint; CVectorC3DVector MovementPoints; float BurnStart, BurnStop; C3DVector FlameStartPos, FlameEndPos;
    CCreatureAction_DragonBurninateInto(CThingCreatureBase* owner, const CVectorC3DVector& mp, const C3DVector& pp, float bs, float be, const C3DVector& fs, const C3DVector& fe)
        : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), PreviousPoint(pp), MovementPoints(mp), BurnStart(bs), BurnStop(be), FlameStartPos(fs), FlameEndPos(fe) {}
    CCreatureAction_DragonBurninateInto(const CCreatureAction_DragonBurninateInto& o)
        : CCombatActionBase(o), PreviousPoint(o.PreviousPoint), MovementPoints(o.MovementPoints), BurnStart(o.BurnStart), BurnStop(o.BurnStop), FlameStartPos(o.FlameStartPos), FlameEndPos(o.FlameEndPos) {}
};
int main()
{
    CThingCreatureBase o;
    CCreatureAction_DragonBurninateInto a(&o, CVectorC3DVector(4), C3DVector(1, 2, 3), 0.5f, 1.5f, C3DVector(4, 5, 6), C3DVector(7, 8, 9));
    CCreatureAction_DragonBurninateInto r(a);
    bool ok = r.owner == &o && r.code == 250 && r.s0 == 9 && !std::strcmp(g_sound, "SWORD_SWING") && r.PreviousPoint.Y == 2
        && r.MovementPoints.n == 4 && r.BurnStart == 0.5f && r.BurnStop == 1.5f && r.FlameStartPos.Z == 6 && r.FlameEndPos.X == 7;
    if (!ok) { std::printf("00953df0_TEST FAIL\n"); return 1; }
    std::printf("00953df0_TEST PASS\n");
    return 0;
}