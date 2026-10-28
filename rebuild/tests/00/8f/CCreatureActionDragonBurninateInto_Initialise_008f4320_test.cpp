// Behaviour test for 0x008F4320 CCreatureAction_DragonBurninateInto::Initialise.
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
struct CVectorC3DVector { int n; };
class CThing;
struct CModeBase { virtual ~CModeBase() {} };
struct CModeDragonStrafeBurninate : CModeBase {
    CThing* owner; const CVectorC3DVector* pts; C3DVector prev, fs, fe; float a, b;
    CModeDragonStrafeBurninate(CThing* o, const CVectorC3DVector& p, const C3DVector& pr, float s, float e, const C3DVector& f1, const C3DVector& f2)
        : owner(o), pts(&p), prev(pr), fs(f1), fe(f2), a(s), b(e) {}
};
class CTCCreatureModeManager { public: CModeBase* added; void AddMode(CModeBase* m) { added = m; } };
class CThing { public:
    bool has; CTCCreatureModeManager mm;
    bool GetInterface(CTCCreatureModeManager** pp, long t) { if (t == 0x31 && has) { *pp = &mm; return true; } return false; }
};
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } operator CThing*() const { return GetPItem(); } };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CCreatureAction_DragonBurninateInto { public:
    CBaseIntelligentPointer PCreature; const char* anim; int baseInit;
    C3DVector PreviousPoint; CVectorC3DVector MovementPoints; float BurnStart, BurnStop; C3DVector FlameStartPos, FlameEndPos;
    void SetCombatAnimation(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        { CCharString name("INTO_BURNINATE"); SetCombatAnimation(name); }
        ++baseInit;
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
            modeManager->AddMode(new CModeDragonStrafeBurninate(PCreature, MovementPoints, PreviousPoint, BurnStart, BurnStop, FlameStartPos, FlameEndPos));
    }
};
int main()
{
    CThing o; o.has = true; o.mm.added = 0;
    CCreatureAction_DragonBurninateInto a; a.PCreature.PItem = &o; a.anim = 0; a.baseInit = 0;
    a.PreviousPoint.X = 1; a.BurnStart = 2; a.BurnStop = 3; a.FlameStartPos.Y = 4; a.FlameEndPos.Z = 5;
    a.Initialise();
    CModeDragonStrafeBurninate* m = (CModeDragonStrafeBurninate*)o.mm.added;
    bool ok = m && m->owner == &o && m->pts == &a.MovementPoints && m->prev.X == 1 && m->a == 2 && m->b == 3 && m->fs.Y == 4 && m->fe.Z == 5
        && !std::strcmp(a.anim, "INTO_BURNINATE") && a.baseInit == 1;
    delete m;
    if (!ok) { std::printf("008f4320_TEST FAIL\n"); return 1; }
    std::printf("008f4320_TEST PASS\n");
    return 0;
}