#include <cstdio>
// Behaviour test for CTCSummonableCreature::OnDie (0x0079C290): the body of src/ over stand-ins -- when a flagged owner is
// killed by a summoned creature of no higher level and a different def, the killer's summoner's spell is told to summon the
// owner's def ahead of it along its velocity, facing back.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
class CThing;
struct CBaseIntelligentPointer {
    CThing* item;
    CThing* GetPItem() const { return item; }
};
struct CPhysicsComponent {
    C3DVector velocity;
    C3DVector GetVelocity() const { return velocity; }
};
class CTCSummonableCreature;
class CTCSummonSpell;
class CThing {
public:
    CPhysicsComponent*     Physics;
    unsigned char          m_flags6c;
    unsigned short         DefIndex;
    CBaseIntelligentPointer Killer;
    C3DVector              pos;
    CTCSummonableCreature* summonable;
    CTCSummonSpell*        spell;
    bool GetInterface(CTCSummonableCreature** ppOut, long type) { if (type != 0xa0 || !summonable) return false; *ppOut = summonable; return true; }
    bool GetInterface(CTCSummonSpell** ppOut, long type) { if (type != 0x91 || !spell) return false; *ppOut = spell; return true; }
    const C3DVector& GetPosition() const { return pos; }
};
class CSummonableCreatureDef { public: float SummonLevel; };
class CTCSummonSpell { public:
    int calls; long def; C3DVector pos; C3DVector facing;
    void SetSummonCreatureGlobalDefNum(long d, const C3DVector& p, const C3DVector& f) { ++calls; def = d; pos = p; facing = f; }
};
class CTCSummonableCreature {
public:
    CThing*                       ParentThing;
    const CSummonableCreatureDef* PDef;
    bool                          HasBeenSummoned;
    CBaseIntelligentPointer       Summoner;
    void OnDie();
};

void CTCSummonableCreature::OnDie()
{
    CThing* owner = ParentThing;
    if (owner->m_flags6c & 8) {
        CThing* killer = (CThing*)owner->Killer.GetPItem();
        CTCSummonableCreature* killerSummonable;
        if (killer && killer->GetInterface(&killerSummonable, 0xa0) && killerSummonable->HasBeenSummoned
            && PDef->SummonLevel >= killerSummonable->PDef->SummonLevel) {
            long defIndex = owner->DefIndex;
            if (defIndex != killer->DefIndex) {
                CThing* summoner = (CThing*)killerSummonable->Summoner.GetPItem();
                CTCSummonSpell* spell;
                if (summoner->GetInterface(&spell, 0x91)) {
                    C3DVector velocity = owner->Physics->GetVelocity();
                    C3DVector spawnPos = velocity + owner->GetPosition();
                    spell->SetSummonCreatureGlobalDefNum(defIndex, spawnPos, -velocity);
                }
            }
        }
    }
}

static int fail(int line) { printf("0079c290_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCSummonSpell spell = { 0 };
    CThing summoner; summoner.spell = &spell; summoner.summonable = 0;
    CSummonableCreatureDef lowDef = { 1.0f }, highDef = { 3.0f };
    CThing killer; killer.DefIndex = 5; killer.spell = 0;
    CTCSummonableCreature killerTC; killerTC.ParentThing = &killer; killerTC.PDef = &lowDef; killerTC.HasBeenSummoned = true;
    killerTC.Summoner.item = &summoner;
    killer.summonable = &killerTC;
    CPhysicsComponent phys; phys.velocity = C3DVector(1, 2, 3);
    CThing owner; owner.Physics = &phys; owner.m_flags6c = 8; owner.DefIndex = 9; owner.Killer.item = &killer;
    owner.pos = C3DVector(10, 20, 30);
    CTCSummonableCreature tc; tc.ParentThing = &owner; tc.PDef = &highDef;
    tc.OnDie();
    CHECK(spell.calls == 1 && spell.def == 9);
    CHECK(spell.pos.X == 11 && spell.pos.Y == 22 && spell.pos.Z == 33);
    CHECK(spell.facing.X == -1 && spell.facing.Y == -2 && spell.facing.Z == -3);
    // same def: nothing
    killer.DefIndex = 9; tc.OnDie(); CHECK(spell.calls == 1);
    killer.DefIndex = 5;
    // killer of higher level: nothing
    tc.PDef = &lowDef; killerTC.PDef = &highDef; tc.OnDie(); CHECK(spell.calls == 1);
    killerTC.PDef = &lowDef; tc.OnDie(); CHECK(spell.calls == 2);          // equal level summons
    // killer not summoned / not summonable / no killer / owner unflagged: nothing
    killerTC.HasBeenSummoned = false; tc.OnDie(); CHECK(spell.calls == 2);
    killerTC.HasBeenSummoned = true; killer.summonable = 0; tc.OnDie(); CHECK(spell.calls == 2);
    killer.summonable = &killerTC; owner.Killer.item = 0; tc.OnDie(); CHECK(spell.calls == 2);
    owner.Killer.item = &killer; owner.m_flags6c = 0; tc.OnDie(); CHECK(spell.calls == 2);
    // summoner without a spell: nothing
    owner.m_flags6c = 8; summoner.spell = 0; tc.OnDie(); CHECK(spell.calls == 2);
    printf("0079c290_TEST PASS\n");
    return 0;
}