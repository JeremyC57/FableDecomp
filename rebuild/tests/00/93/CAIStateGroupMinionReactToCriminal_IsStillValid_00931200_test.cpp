// Behaviour test for 0x00931200 CAIStateGroup_MinionReactToCriminal::IsStillValid() const: a copy of the model with the same logic.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    bool Dead;
    bool Guard;                    // HasTC(TCI_GUARD)
    C3DVector Pos;
    bool IsDead() const { return Dead; }
    const C3DVector& GetPos() const { return Pos; }
};
class CThingBuilding : public CThing { public:   // axis-aligned footprint
    float MinX, MaxX, MinY, MaxY;
    bool IsPointInside(const C3DVector& p) const { return p.X >= MinX && p.X <= MaxX && p.Y >= MinY && p.Y <= MaxY; }
};
template<class T> class CIntelligentPointer { public:
    T* PItem;
    T* GetPItem() const { return PItem; }
    T* operator->() const { return PItem; }
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing->IsDead()) return true;
    }
    return false;
}
struct CPersonNode { CPersonNode* Next; CIntelligentPointer<CThing> Value; };
class CTCVillage { public: CPersonNode* VillagePeople; };
class CTCVillageMember { public:
    CTCVillage* PVillage;
    CTCVillage* GetPVillage() const { return PVillage; }
};
struct CCreature : CThing {
    CTCVillageMember* VillageMember;        // 0 = no TCI_VILLAGE_MEMBER component
    bool GetTC(CTCVillageMember** out) { if (VillageMember) { *out = VillageMember; return true; } return false; }
};
class CAIBrain { public: CCreature* Creature; };

struct CStateGroup {
    CAIBrain* PBrain;
    CIntelligentPointer<CThing> PHero;
    CIntelligentPointer<CThingBuilding> PHome;
    mutable bool HeroInsideHouse, MeInsideHouse, GuardInsideHouse;
    bool Valid;
    mutable int IsValidCalls;
    bool IsValid() const { ++IsValidCalls; return Valid; }
    bool IsStillValid() const
    {
        if (IS_THING_ALIVE(PHome)) {
            CCreature& creature = *PBrain->Creature;
            MeInsideHouse = PHome->IsPointInside(creature.GetPos());
            HeroInsideHouse = IS_THING_ALIVE(PHero) && PHome->IsPointInside(PHero->GetPos());
            GuardInsideHouse = false;
            CTCVillageMember* village_member;
            if (PBrain->Creature->GetTC(&village_member) && village_member->GetPVillage()) {
                for (CPersonNode* it = village_member->GetPVillage()->VillagePeople; it != 0; it = it->Next) {
                    const CIntelligentPointer<CThing>& person = it->Value;
                    if (IS_THING_ALIVE(person) && person->Guard && PHome->IsPointInside(person->GetPos())) {
                        GuardInsideHouse = true;
                        break;
                    }
                }
            }
        }
        return IsValid();
    }
};

static void SetPos(CThing& t, float x, float y) { t.Pos.X = x; t.Pos.Y = y; t.Pos.Z = 0.0f; }

int main()
{
    int bad = 0;
    CThingBuilding home; home.Dead = false; home.Guard = false; SetPos(home, 5, 5);
    home.MinX = 0; home.MaxX = 10; home.MinY = 0; home.MaxY = 10;
    CThing hero; hero.Dead = false; hero.Guard = false; SetPos(hero, 3, 3);
    CThing guardOut; guardOut.Dead = false; guardOut.Guard = true; SetPos(guardOut, 50, 50);
    CThing guardIn; guardIn.Dead = false; guardIn.Guard = true; SetPos(guardIn, 1, 9);
    CThing villagerIn; villagerIn.Dead = false; villagerIn.Guard = false; SetPos(villagerIn, 2, 2);
    CPersonNode n3 = { 0, { &guardIn } };
    CPersonNode n2 = { &n3, { &guardOut } };
    CPersonNode n1 = { &n2, { &villagerIn } };
    CPersonNode n0 = { &n1, { 0 } };                      // an empty pointer is skipped
    CTCVillage village = { &n0 };
    CTCVillageMember member = { &village };
    CCreature me; me.Dead = false; me.Guard = false; SetPos(me, 20, 20); me.VillageMember = &member;
    CAIBrain brain = { &me };
    CStateGroup g; g.PBrain = &brain; g.PHero.PItem = &hero; g.PHome.PItem = &home;
    g.Valid = true; g.IsValidCalls = 0;

    // 1. everything live: me outside, hero inside, a guard inside
    g.HeroInsideHouse = false; g.MeInsideHouse = true; g.GuardInsideHouse = false;
    if (!g.IsStillValid() || g.MeInsideHouse || !g.HeroInsideHouse || !g.GuardInsideHouse) bad |= 1;

    // 2. me inside, hero dead, the inside guard dead -> only MeInsideHouse; result follows IsValid
    SetPos(me, 5, 5); hero.Dead = true; guardIn.Dead = true; g.Valid = false;
    if (g.IsStillValid() || !g.MeInsideHouse || g.HeroInsideHouse || g.GuardInsideHouse) bad |= 2;

    // 3. hero alive but outside; a non-guard inside does not count; no hero pointer at all is not inside either
    hero.Dead = false; SetPos(hero, -1, 3); g.Valid = true;
    if (!g.IsStillValid() || g.HeroInsideHouse || g.GuardInsideHouse) bad |= 4;
    g.PHero.PItem = 0; g.HeroInsideHouse = true;
    if (!g.IsStillValid() || g.HeroInsideHouse) bad |= 4;
    g.PHero.PItem = &hero; SetPos(hero, 3, 3);

    // 4. the outside guard walks in -> found (first match breaks)
    SetPos(guardOut, 9, 1);
    if (!g.IsStillValid() || !g.GuardInsideHouse || !g.HeroInsideHouse) bad |= 8;

    // 5. no village-member component / no village: the guard flag is cleared, the other two still refreshed
    me.VillageMember = 0; g.GuardInsideHouse = true;
    if (!g.IsStillValid() || g.GuardInsideHouse || !g.MeInsideHouse) bad |= 16;
    me.VillageMember = &member; member.PVillage = 0; g.GuardInsideHouse = true;
    if (!g.IsStillValid() || g.GuardInsideHouse) bad |= 16;
    member.PVillage = &village;

    // 6. home dead or missing: the flags are left alone, IsValid still decides
    home.Dead = true; g.HeroInsideHouse = false; g.MeInsideHouse = false; g.GuardInsideHouse = false; g.Valid = false;
    int calls = g.IsValidCalls;
    if (g.IsStillValid() || g.HeroInsideHouse || g.MeInsideHouse || g.GuardInsideHouse || g.IsValidCalls != calls + 1) bad |= 32;
    g.PHome.PItem = 0; g.Valid = true; g.GuardInsideHouse = true;
    if (!g.IsStillValid() || !g.GuardInsideHouse || g.MeInsideHouse) bad |= 32;

    if (g.IsValidCalls != 9) bad |= 64;                    // exactly one IsValid per call
    if (bad) { std::printf("00931200_TEST FAIL (%d)\n", bad); return 1; }
    std::printf("00931200_TEST PASS\n");
    return 0;
}