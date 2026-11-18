// Standalone C++03 fixture for the recovered dice-game admission predicate.
#include <cstdio>
#include <list>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};

struct CThing;
struct CTCExclusiveUse {
    CThing* User;
    CTCExclusiveUse() : User(0) {}
    CThing* GetUser() const { return User; }
};

struct CTCVillageMember;
enum ETCInterfaceType { TCI_VILLAGE_MEMBER };
struct CThing {
    C3DVector Position;
    bool Dead;
    long Definition;
    CTCExclusiveUse* Use;
    CTCVillageMember* Village;
    CThing(float x, long definition) : Position(x, 0, 0), Dead(false), Definition(definition), Use(0), Village(0) {}

    const C3DVector& GetPos() const { return Position; }
    bool IsDead() const { return Dead; }
    long GetDefGlobalIndex() const { return Definition; }
    bool GetTC(ETCInterfaceType, CTCVillageMember** out) const { *out = Village; return Village != 0; }
    bool GetTC(CTCExclusiveUse** out) const { *out = Use; return Use != 0; }
};

struct CThingMarker : CThing {
    CThingMarker(float x, long def) : CThing(x, def) {}
};

template<class T> struct CIntelligentPointer {
    T* Ptr;
    explicit CIntelligentPointer(T* ptr) : Ptr(ptr) {}
    T* Get() const { return Ptr; }
    T* operator->() const { return Ptr; }
};

struct CTCVillage {
    std::list<CIntelligentPointer<CThingMarker> > VillageMarkers;
};

struct CTCVillageMember {
    CTCVillage* Village;
    CTCVillage* GetPVillage() const { return Village; }
};

struct CAIBrain {
    CThing* Creature;
};

struct CTCDiceGame {};

struct CWorld {
    static long Frame;
    static long GetFrame() { return Frame; }
};
long CWorld::Frame = 0;

struct CAIStateGroup_BanditPlayDice {
    CTCDiceGame* Available;
    CTCDiceGame* DiceGame;
    CTCExclusiveUse* DiceGamePosition;
    CAIBrain* PBrain;
    long FrameToInitiate, FrameToJoin, MarkerType;
    int GameLookups;

    explicit CAIStateGroup_BanditPlayDice(CAIBrain& brain)
        : Available(0), DiceGame(0), DiceGamePosition(0), PBrain(&brain),
          FrameToInitiate(10), FrameToJoin(20), MarkerType(42), GameLookups(0) {}
    CTCDiceGame* GetDiceGame() { ++GameLookups; return Available; }
    long GetMarkerType() const { return MarkerType; }
    bool IsRunnable();
};

// Recovered method body, with game dependencies supplied by the fixture.
bool CAIStateGroup_BanditPlayDice::IsRunnable()
{
    if (CWorld::GetFrame() < FrameToInitiate)
        return false;
    DiceGame = GetDiceGame();
    if (!DiceGame)
        return false;
    bool someone_already_playing = false;
    CThing* best = 0;
    float best_dist = 0.0f;
    long marker_type = GetMarkerType();
    CTCVillageMember* village_member;
    if (!PBrain->Creature->GetTC(TCI_VILLAGE_MEMBER, &village_member))
        return false;
    const std::list<CIntelligentPointer<CThingMarker> >& markers = village_member->GetPVillage()->VillageMarkers;
    for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i = markers.begin(); i != markers.end(); ++i) {
        if (i->Get() && !(*i)->IsDead() && (*i)->GetDefGlobalIndex() == marker_type) {
            CTCExclusiveUse* exclusive_use;
            (*i)->GetTC(&exclusive_use);
            if (exclusive_use->GetUser()) {
                someone_already_playing = true;
            } else {
                CThing* creature = PBrain->Creature;
                float dist = ((*i)->GetPos() - creature->GetPos()).GetSquaredMagnitude();
                if (!best || dist < best_dist) {
                    best = i->Get();
                    best_dist = dist;
                    DiceGamePosition = exclusive_use;
                }
            }
        }
    }
    if (best && (someone_already_playing || CWorld::GetFrame() > FrameToJoin))
        return true;
    return false;
}

int main()
{
    CTCDiceGame game;
    CThing actor(0.0f, 0);
    CAIBrain brain = { &actor };
    CAIStateGroup_BanditPlayDice owner(brain);
    CTCVillage village;
    CTCVillageMember member = { &village };

    CWorld::Frame = 9;
    bool ok = !owner.IsRunnable() && owner.GameLookups == 0;
    CWorld::Frame = 10;
    ok = ok && !owner.IsRunnable() && owner.GameLookups == 1;
    owner.Available = &game;
    ok = ok && !owner.IsRunnable() && owner.DiceGame == &game;
    actor.Village = &member;
    ok = ok && !owner.IsRunnable() && owner.DiceGamePosition == 0;

    CThingMarker near_marker(2, 42), far_marker(5, 42), taken(1, 42), wrong(1, 12), dead(1, 42);
    CTCExclusiveUse near_use, far_use, taken_use;
    near_marker.Use = &near_use;
    far_marker.Use = &far_use;
    taken.Use = &taken_use;
    taken_use.User = &actor;
    dead.Dead = true;
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(0));
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&wrong));
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&dead));
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&far_marker));
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&near_marker));

    CWorld::Frame = 20;
    ok = ok && !owner.IsRunnable() && owner.DiceGamePosition == &near_use;
    CWorld::Frame = 21;
    ok = ok && owner.IsRunnable() && owner.DiceGamePosition == &near_use;
    CWorld::Frame = 10;
    village.VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&taken));
    ok = ok && owner.IsRunnable() && owner.DiceGamePosition == &near_use;
    near_use.User = &actor;
    ok = ok && owner.IsRunnable() && owner.DiceGamePosition == &far_use;
    far_use.User = &actor;
    ok = ok && !owner.IsRunnable();

    std::puts(ok ? "00929020_TEST PASS" : "00929020_TEST FAIL");
    return ok ? 0 : 1;
}
