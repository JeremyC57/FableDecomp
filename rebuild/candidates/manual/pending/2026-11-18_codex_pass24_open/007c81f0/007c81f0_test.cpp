#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x=0, float y=0, float z=0): X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X+v.X,Y+v.Y,Z+v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X-v.X,Y-v.Y,Z-v.Z); }
    float Dot(const C3DVector& v) const { return X*v.X+Y*v.Y+Z*v.Z; }
    void Normalise()
    {
        const float length = static_cast<float>(std::sqrt(Dot(*this)));
        assert(length > 0);
        X /= length;
        Y /= length;
        Z /= length;
    }
    void RotateXY(float turns)
    {
        const float original_x = X;
        if (turns == -0.25f) {
            X = Y;
            Y = -original_x;
        } else {
            assert(turns == 0.5f);
            X = -X;
            Y = -Y;
        }
    }
};

bool SameVector(const C3DVector& lhs, const C3DVector& rhs)
{
    return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
}

struct CThing;
struct CTCSolidToThing
{
    unsigned Calls;
    CThing* Last;
    CTCSolidToThing(): Calls(0), Last(0) {}
    void AddThingNotSolidTo(CThing* thing) { ++Calls; Last = thing; }
};

struct CTCPhysicsBase
{
    unsigned FacingReads;
    CTCPhysicsBase(): FacingReads(0) {}
    C3DVector GetFacingVector() { ++FacingReads; return C3DVector(1,0,0); }
};

enum { TCI_SOLID_TO_THING };
struct CThing
{
    unsigned TypeFlags, Adds, Lookups;
    bool HasSolid, CanAttach;
    C3DVector Pos;
    CTCPhysicsBase Physics;
    CTCSolidToThing Solid;
    CThing(): TypeFlags(0), Adds(0), Lookups(0), HasSolid(false), CanAttach(false) {}
    unsigned GetTypeFlags() const { return TypeFlags; }
    const C3DVector& GetPos() const { return Pos; }
    CTCPhysicsBase* GetPhysicsTC() { return &Physics; }
    bool HasTC(int type) const { assert(type == TCI_SOLID_TO_THING); return HasSolid; }
    void AddTC(const char* name, bool defer, int flags)
    {
        assert(std::strcmp(name, "CTCSolidToThing") == 0 && !defer && flags == 0);
        ++Adds;
        HasSolid = CanAttach;
    }
    bool GetTC(CTCSolidToThing** component)
    {
        ++Lookups;
        *component = HasSolid ? &Solid : 0;
        return HasSolid;
    }
};

enum EDamageAttribute { DAMAGE_ATTRIBUTE_FIXTURE };
struct CHitParameters
{
    CThing* Owner;
    CThing* Weapon;
    C3DVector Pos, Direction;
    float Damage;
    bool Blockable, Blocked, Negated;
    CHitParameters(int first, CThing* owner, int third, const C3DVector& pos,
        const C3DVector& direction, bool valid, bool a, bool b, bool c)
        : Owner(owner), Weapon(0), Pos(pos), Direction(direction), Damage(0),
          Blockable(false), Blocked(false), Negated(false)
    { assert(!first && !third && valid && !a && !b && !c); }
    void AddDamageWithAttribute(EDamageAttribute type, float amount)
    {
        assert(type == DAMAGE_ATTRIBUTE_FIXTURE);
        Damage += amount;
    }
    void SetBlockable(bool blockable) { Blockable = blockable; }
    void SetWeapon(CThing& weapon) { Weapon = &weapon; }
    bool IsBlocked() const { return Blocked; }
    bool IsHitNegated() const { return Negated; }
};

struct CThingGameObject : CThing
{
    unsigned Hits;
    bool Blocks, Negates;
    float Damage;
    bool Blockable;
    CThing* ExpectedOwner;
    C3DVector HitPos, HitDirection;
    CThingGameObject(): Hits(0), Blocks(false), Negates(false), Damage(0),
        Blockable(false), ExpectedOwner(0) {}
    void OnHit(CHitParameters& params)
    {
        assert(params.Owner == ExpectedOwner && params.Weapon == ExpectedOwner);
        ++Hits;
        Damage = params.Damage;
        Blockable = params.Blockable;
        HitPos = params.Pos;
        HitDirection = params.Direction;
        params.Blocked = Blocks;
        params.Negated = Negates;
    }
};

struct CCreatureActionBase
{
    bool Strike;
    bool IsStrikeAction() const { return Strike; }
};

struct CThingCreatureBase : CThingGameObject
{
    bool HasAction;
    CCreatureActionBase Action;
    const CCreatureActionBase* PeekPCurrentAction() const { return HasAction ? &Action : 0; }
};

struct CTCHitOnCollision
{
    CThing Parent;
    std::vector<CThingGameObject*> CollidedList;
    bool DontHitWhenCombatAction, PushSideways, Blockable, PassThroughIfBlocked;
    float Damage;
    CThing& GetParentThing() { return Parent; }
    void OnCollision(const C3DVector& collide_pos, const C3DVector& collision_normal, CThing& target);
};
void CTCHitOnCollision::OnCollision(const C3DVector& collide_pos, const C3DVector& collision_normal, CThing& target)
{
    if (target.GetTypeFlags() & 8) {
        CThingGameObject& targetGameObject = static_cast<CThingGameObject&>(target);
        if (std::find(CollidedList.begin(), CollidedList.end(), &targetGameObject) == CollidedList.end()) {
            bool doHit = true;
            if (DontHitWhenCombatAction) {
                CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(target);
                const CCreatureActionBase* pAction = creature.PeekPCurrentAction();
                if (pAction && pAction->IsStrikeAction())
                    doHit = false;
            }
            if (doHit) {
                C3DVector hit_direction = collision_normal;
                C3DVector hit_pos = collide_pos;
                if (PushSideways) {
                    C3DVector left = GetParentThing().GetPhysicsTC()->GetFacingVector();
                    left.RotateXY(-0.25f);
                    C3DVector vector_to_target = target.GetPos() - GetParentThing().GetPos();
                    vector_to_target.Normalise();
                    hit_direction = left;
                    if (left.Dot(vector_to_target) > 0.0f)
                        hit_direction.RotateXY(0.5f);
                    hit_pos = hit_direction + target.GetPos();
                }
                CHitParameters params(0, &GetParentThing(), 0, hit_pos, hit_direction, true, false, false, false);
                params.AddDamageWithAttribute((EDamageAttribute)0, Damage);
                params.SetBlockable(Blockable);
                params.SetWeapon(GetParentThing());
                targetGameObject.OnHit(params);
                if (PassThroughIfBlocked || (!params.IsBlocked() && !params.IsHitNegated())) {
                    CollidedList.push_back(&targetGameObject);
                    if (!GetParentThing().HasTC(TCI_SOLID_TO_THING))
                        GetParentThing().AddTC("CTCSolidToThing", false, 0);
                    CTCSolidToThing* pTCSolidToThing;
                    if (GetParentThing().GetTC(&pTCSolidToThing))
                        pTCSolidToThing->AddThingNotSolidTo(&target);
                }
            }
        }
    }
}

int main()
{
    const C3DVector contact(7,8,9), normal(0,0,1);
    for (unsigned mask = 0; mask < 8192; ++mask) {
        CTCHitOnCollision hit;
        CThingCreatureBase target;
        target.TypeFlags = mask & 1 ? 8 : 0;
        target.HasAction = (mask & 2) != 0;
        target.Action.Strike = (mask & 4) != 0;
        target.Blocks = (mask & 8) != 0;
        target.Negates = (mask & 16) != 0;
        target.ExpectedOwner = &hit.Parent;
        target.Pos = C3DVector(2,mask & 4096 ? -3.0f : 3.0f,1);
        hit.DontHitWhenCombatAction = (mask & 32) != 0;
        hit.Blockable = (mask & 64) != 0;
        hit.PassThroughIfBlocked = (mask & 128) != 0;
        hit.PushSideways = (mask & 256) != 0;
        hit.Parent.HasSolid = (mask & 512) != 0;
        hit.Parent.CanAttach = (mask & 1024) != 0;
        hit.Damage = 37.0f;
        const bool duplicate = (mask & 2048) != 0;
        if (duplicate)
            hit.CollidedList.push_back(&target);

        const bool hit_expected = target.TypeFlags && !duplicate &&
            !(hit.DontHitWhenCombatAction && target.HasAction && target.Action.Strike);
        const bool retain = hit_expected && (hit.PassThroughIfBlocked || (!target.Blocks && !target.Negates));
        const bool add_solid = retain && !hit.Parent.HasSolid;
        const bool make_not_solid = retain && (hit.Parent.HasSolid || hit.Parent.CanAttach);
        hit.OnCollision(contact, normal, target);
        assert(target.Hits == hit_expected);
        assert(hit.Parent.Physics.FacingReads == (hit_expected && hit.PushSideways));
        assert(hit.Parent.Adds == add_solid && hit.Parent.Lookups == retain);
        assert(hit.Parent.Solid.Calls == make_not_solid);
        assert(hit.Parent.Solid.Last == (make_not_solid ? &target : 0));
        assert(hit.CollidedList.size() == unsigned(duplicate || retain));
        if (duplicate || retain)
            assert(hit.CollidedList.front() == &target);
        if (hit_expected) {
            assert(target.Damage == 37.0f && target.Blockable == hit.Blockable);
            const C3DVector sideways(0,mask & 4096 ? 1.0f : -1.0f,0);
            assert(SameVector(target.HitDirection,hit.PushSideways ? sideways : normal));
            assert(SameVector(target.HitPos,hit.PushSideways ? target.Pos+sideways : contact));
        }
    }
    std::puts("007c81f0_TEST PASS");
    return 0;
}
