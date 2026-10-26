// Behaviour test for CCreatureAction_DragonTakeOffIntoFlying::FinishAction (0x008F47E0): flying on, component 0x4D removed,
// dragon dropped from the hero's 0x4D list, lifted 31.148378 units, the lifted point inserted at the front of
// MovementPoints, and with a mode manager PreviousPoint = first point and a CModeDragonFlying(StartPos, 1.0, PEndAction)
// added; the base FinishAction always runs.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; };
class CThing;
static int g_flying = 0, g_removedTC = 0, g_listRemoved = 0, g_baseFinish = 0, g_modesAdded = 0;
static C3DVector g_setPos; static float g_modeSpeed = 0; static C3DVector g_modePos; static int g_modeAction = 0;
static CThing* g_modeCreature = 0; static int g_modePathLen = 0;
struct CPhysicsBody {
    void SetPosition(const C3DVector& p) { g_setPos = p; }
    void SetFlying(bool f) { g_flying = f ? 1 : 2; }
};
struct CTC4D { CThing* Removed; void RemoveThing(CThing* t) { Removed = t; ++g_listRemoved; } };
class CTCCreatureModeManager;
class CThing { public:
    CPhysicsBody* PhysicsBody; C3DVector Pos; CTC4D* List; CTCCreatureModeManager* Modes;
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const { return Pos; }
    void RemoveTC(long t) { g_removedTC = (int)t; }
    bool GetInterface(CTC4D** out, long) { if (!List) return false; *out = List; return true; }
    bool GetInterface(CTCCreatureModeManager** out, long) { if (!Modes) return false; *out = Modes; return true; }
};
class CPlayer { public: CThing* Hero; CThing* GetHeroCreature() { return Hero; } };
class CPlayerManager { public: CPlayer* Main; CPlayer* GetMainPlayer() { return Main; } };
class CMainGameComponent { public: CPlayerManager* PlayerManager; };
static CMainGameComponent* g_pMainGameComponent;
struct C3DVectorVector {
    C3DVector Data[8]; int Size;
    C3DVector* begin() { return Data; }
    C3DVector& operator[](long i) { return Data[i]; }
    C3DVector* insert(C3DVector* where, const C3DVector& v) { long i = (long)(where - Data); for (int j = Size; j > i; --j) Data[j] = Data[j - 1]; Data[i] = v; ++Size; return Data + i; }
};
struct CCountedActionPointer { int Id; };
struct CModeDragonFlying {
    CModeDragonFlying(CThing* c, const C3DVectorVector& path, const C3DVector& pos, float speed, CCountedActionPointer a)
    { g_modeCreature = c; g_modePathLen = path.Size; g_modePos = pos; g_modeSpeed = speed; g_modeAction = a.Id; }
};
struct CCountedModePointer { CModeDragonFlying* m_pObject; CCountedModePointer(CModeDragonFlying* o) : m_pObject(o) {} };
class CTCCreatureModeManager { public: bool AddMode(CCountedModePointer m) { ++g_modesAdded; delete m.m_pObject; return true; } };
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public:
    CPtr PCreature;
    void FinishAction() { ++g_baseFinish; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    C3DVector Dest, StartPos, PreviousPoint; C3DVectorVector MovementPoints; bool UsePreviousPoint; CCountedActionPointer PEndAction;
    void FinishAction();
};

void CCreatureAction_DragonTakeOffIntoFlying::FinishAction()
{
    PCreature.GetPItem()->GetPhysics()->SetFlying(true);
    PCreature.GetPItem()->RemoveTC(0x4d);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroCreature();
    CTC4D* list;
    if (hero->GetInterface(&list, 0x4d))
        list->RemoveThing(PCreature.GetPItem());
    const C3DVector& position = PCreature.GetPItem()->GetPosition();
    C3DVector lifted;
    lifted.X = position.X;
    lifted.Y = position.Y;
    lifted.Z = position.Z + 31.148378f;
    PCreature.GetPItem()->GetPhysics()->SetPosition(lifted);
    MovementPoints.insert(MovementPoints.begin(), lifted);
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31)) {
        PreviousPoint = MovementPoints[0];
        modes->AddMode(CCountedModePointer(new CModeDragonFlying(GetCreature(), MovementPoints, StartPos, 1.0f, PEndAction)));
    }
    CCombatActionBase::FinishAction();
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    int ok = 1;
    CPhysicsBody body; CTC4D list; list.Removed = 0; CTCCreatureModeManager modes;
    CThing dragon; dragon.PhysicsBody = &body; dragon.Pos.X = 1; dragon.Pos.Y = 2; dragon.Pos.Z = 3; dragon.List = 0; dragon.Modes = &modes;
    CThing hero; hero.PhysicsBody = 0; hero.List = &list; hero.Modes = 0;
    CPlayer player; player.Hero = &hero; CPlayerManager pm; pm.Main = &player; CMainGameComponent mg; mg.PlayerManager = &pm;
    g_pMainGameComponent = &mg;
    CCreatureAction_DragonTakeOffIntoFlying a; a.PCreature.m_pItem = &dragon;
    a.StartPos.X = 7; a.StartPos.Y = 8; a.StartPos.Z = 9; a.PreviousPoint.X = a.PreviousPoint.Y = a.PreviousPoint.Z = 0;
    a.MovementPoints.Size = 1; a.MovementPoints.Data[0].X = 50; a.MovementPoints.Data[0].Y = 60; a.MovementPoints.Data[0].Z = 70;
    a.PEndAction.Id = 42;
    a.FinishAction();
    if (g_flying != 1 || g_removedTC != 0x4d || list.Removed != &dragon || g_baseFinish != 1) ok = 0;
    if (!near(g_setPos.X, 1) || !near(g_setPos.Y, 2) || !near(g_setPos.Z, 34.148378f)) ok = 0;
    if (a.MovementPoints.Size != 2 || !near(a.MovementPoints.Data[0].Z, 34.148378f) || !near(a.MovementPoints.Data[1].X, 50)) ok = 0;
    if (!near(a.PreviousPoint.Z, 34.148378f) || g_modesAdded != 1 || g_modeCreature != &dragon || g_modePathLen != 2) ok = 0;
    if (!near(g_modePos.X, 7) || !near(g_modeSpeed, 1.0f) || g_modeAction != 42) ok = 0;
    // no mode manager and no hero list: still lifts and inserts, no mode, base still runs
    dragon.Modes = 0; hero.List = 0; list.Removed = 0;
    a.FinishAction();
    if (g_modesAdded != 1 || g_listRemoved != 1 || a.MovementPoints.Size != 3 || g_baseFinish != 2) ok = 0;
    std::printf("008f47e0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}