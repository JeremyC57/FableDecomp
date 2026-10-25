// Behaviour test for CWorld::SetSaveGameMarkerPos (0x0049E590): the position is always stored; the marker angle takes
// the hero's facing angle only when there is a hero and it is not dead.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
struct CTCPhysicsBase {
    float Angle;
    virtual ~CTCPhysicsBase() {}
    virtual float GetAngleXY() const { return Angle; }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC;
    unsigned char   Dead : 1;
};
struct CPlayer { CThing* Hero; CThing* GetHeroThing() { return Hero; } };
struct CPlayerManager { CPlayer* Main; CPlayer* GetMainPlayer() { return Main; } };

class CWorld {
public:
    void SetSaveGameMarkerPos(const C3DVector& pos);
    CPlayerManager* PlayerManager;
    C3DVector       SaveGameMarkerPos;
    float           SaveGameMarkerAngleXY;
};

void CWorld::SetSaveGameMarkerPos(const C3DVector& pos)
{
    SaveGameMarkerPos = pos;
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero && !hero->Dead)
        SaveGameMarkerAngleXY = hero->PhysicsTC->GetAngleXY();
}

int main()
{
    CTCPhysicsBase physics; physics.Angle = 1.5f;
    CThing hero; hero.PhysicsTC = &physics; hero.Dead = 1;
    CPlayer player; player.Hero = 0;
    CPlayerManager pm; pm.Main = &player;
    CWorld w; w.PlayerManager = &pm; w.SaveGameMarkerAngleXY = -1.0f;
    C3DVector p1 = { 1.0f, 2.0f, 3.0f }, p2 = { 4.0f, 5.0f, 6.0f }, p3 = { 7.0f, 8.0f, 9.0f };
    w.SetSaveGameMarkerPos(p1);                                   /* no hero */
    bool ok = w.SaveGameMarkerPos.X == 1.0f && w.SaveGameMarkerPos.Z == 3.0f && w.SaveGameMarkerAngleXY == -1.0f;
    player.Hero = &hero;
    w.SetSaveGameMarkerPos(p2);                                   /* dead hero */
    ok = ok && w.SaveGameMarkerPos.Y == 5.0f && w.SaveGameMarkerAngleXY == -1.0f;
    hero.Dead = 0;
    w.SetSaveGameMarkerPos(p3);                                   /* live hero */
    ok = ok && w.SaveGameMarkerPos.X == 7.0f && w.SaveGameMarkerPos.Z == 9.0f && w.SaveGameMarkerAngleXY == 1.5f;
    if (!ok) { std::printf("0049e590_TEST FAIL\n"); return 1; }
    std::printf("0049e590_TEST PASS\n");
    return 0;
}