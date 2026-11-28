// Deterministic engine-interface models for the action-event method.
#include <cassert>
#include <cstdio>
#include <cstring>
#define FABLE_PORTABLE
#include "fable_assert.h"
#include "engine/EWeaponClass.h"

struct Scenario {
    const char* Name;
    bool Strike;
    int Target;
    bool SourceDef;
    long ShotIndex;
    bool Combat;
    long OverrideIndex;
    int Carried;
    int StrikeWeapon;
    bool DummySucceeds;
    bool HelperSucceeds;
    bool CreateSucceeds;
    long ExpectedIndex;
    const char* Expected;
};

static const Scenario* Case;
static char Calls[40];
static unsigned CallCount;
static long CreatedIndex;
static unsigned Shots;

static void Log(char call)
{
    assert(CallCount + 1 < sizeof(Calls));
    Calls[CallCount++] = call;
    Calls[CallCount] = 0;
}

class FixtureCharString {
public:
    const char* Text;
    FixtureCharString(const char* text) : Text(text) {}
};

struct Fixture3DVector {
    float X;
    float Y;
    float Z;
    Fixture3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    Fixture3DVector operator+(const Fixture3DVector& rhs) const
    {
        return Fixture3DVector(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
    }
};

struct FixtureRightHandedSet {
    int Angle;
    explicit FixtureRightHandedSet(int angle) : Angle(angle) {}
};

struct DefReferences {
    mutable int References;
    DefReferences() : References(0) {}
};

struct FixtureWeaponDef : DefReferences {
    EWeaponClass Class;
    long Shot;
    FixtureWeaponDef(EWeaponClass type, long shot) : Class(type), Shot(shot) {}
};

struct FixtureMeleeCombatAbilityDef : DefReferences {};

template<class T> class FixtureDefPointer {
    T* Object;
public:
    FixtureDefPointer() : Object(0) {}
    explicit FixtureDefPointer(T* object) : Object(object) { if (Object) ++Object->References; }
    FixtureDefPointer(const FixtureDefPointer& other) : Object(other.Object) { if (Object) ++Object->References; }
    ~FixtureDefPointer() { if (Object) --Object->References; }
    void Assign(T* object)
    {
        assert(!Object);
        Object = object;
        if (Object) ++Object->References;
    }
    T* operator->() const { return Object; }
};

struct FixtureThingBaseDef : DefReferences {
    const FixtureWeaponDef* Weapon;
    bool IsCarried;
    FixtureThingBaseDef(const FixtureWeaponDef* weapon, bool is_carried)
        : Weapon(weapon), IsCarried(is_carried) {}
    bool GetSubDef(FixtureDefPointer<const FixtureWeaponDef>* out) const
    {
        Log(IsCarried ? 'd' : 'D');
        if (IsCarried)
            assert(References == 1);
        out->Assign(Weapon);
        return Weapon != 0;
    }
};

struct FixturePhysics {
    bool IsCarried;
    Fixture3DVector Position;
    FixtureRightHandedSet Orientation;
    FixturePhysics() : IsCarried(false), Position(2, 4, 6), Orientation(15) {}
    const Fixture3DVector& GetPosition() const { Log('G'); return Position; }
    const FixtureRightHandedSet& GetRHSet() const { Log(IsCarried ? 'r' : 'R'); return Orientation; }
    void SetRHSet(const FixtureRightHandedSet& rhs)
    {
        Log('O');
        assert(IsCarried && Orientation.Angle == 7 && rhs.Angle == 45);
        Orientation = rhs;
    }
};

struct FixtureGraphicAppearance {
    bool GetDummyObject(const FixtureCharString& name, long index, unsigned long flags,
        Fixture3DVector& pos, FixtureRightHandedSet& rhs, float scale)
    {
        Log('M');
        assert(std::strcmp(name.Text, "WEAPON_FOCUS_01") == 0 && !index && !flags && scale == 1.0f);
        if (!Case->DummySucceeds)
            return false;
        pos = Fixture3DVector(20, 40, 60);
        rhs.Angle = 25;
        return true;
    }
    bool GetHelperPoint(const FixtureCharString& name, long index, unsigned long flags,
        Fixture3DVector& pos, float scale)
    {
        Log('J');
        assert(std::strcmp(name.Text, "strike_helper") == 0 && !index && !flags && scale == 1.0f);
        if (!Case->HelperSucceeds)
            return false;
        pos = Fixture3DVector(10, 30, 50);
        return true;
    }
};

struct FixtureThing {
    bool Dead;
    FixtureThingBaseDef* BaseDef;
    FixturePhysics Physics;
    FixtureGraphicAppearance Graphic;
    FixtureThing() : Dead(false), BaseDef(0) {}
    FixturePhysics* GetPhysicsTC() { return &Physics; }
    FixtureGraphicAppearance* GetTCGraphicAppearance() { return &Graphic; }
    FixtureDefPointer<const FixtureThingBaseDef> GetPBaseDef() const
    {
        Log('B');
        return FixtureDefPointer<const FixtureThingBaseDef>(BaseDef);
    }
};

template<class T> static bool IS_THING_ALIVE(T* const& thing)
{
    return thing && !thing->Dead;
}

struct FixtureTCCombat {
    long GetShotTypeOverride() const { return Case->OverrideIndex; }
};

struct FixtureTCCarrying {
    FixtureThing* Carried;
    explicit FixtureTCCarrying(FixtureThing* carried) : Carried(carried) {}
    FixtureThing* GetThingInPrimarySlot() const { Log('P'); return Carried; }
    void RemoveThing(FixtureThing& thing)
    {
        Log('X');
        assert(Carried == &thing && thing.Physics.Orientation.Angle == 45);
        thing.Physics.Orientation.Angle = 7;
        Carried = 0;
    }
};

struct Creature : FixtureThing {
    FixtureTCCombat CombatComponent;
    FixtureTCCarrying Carrying;
    explicit Creature(FixtureThing* carried) : Carrying(carried) {}
    const FixtureThingBaseDef& PeekBaseDef() const { return *BaseDef; }
    void MakeNoise(const FixtureCharString& name)
    {
        Log('N');
        assert(std::strcmp(name.Text, "SIMPLE_NOISE") == 0);
    }
    bool PeekTC(const FixtureTCCombat** out) const
    {
        Log('C');
        if (Case->Combat)
            *out = &CombatComponent;
        return Case->Combat;
    }
    long GetShotAccuracyPercentage() const { Log('A'); return 38; }
    bool GetTC(FixtureTCCarrying** out)
    {
        Log('Y');
        if (Case->Carried)
            *out = &Carrying;
        return Case->Carried != 0;
    }
};

struct FixtureCreatureActionBase {
    virtual ~FixtureCreatureActionBase() {}
    virtual bool IsStrikeAction() const { Log('S'); return Case->Strike; }
};

struct FixtureCombatActionBase : FixtureCreatureActionBase {
    FixtureThing* Target;
    FixtureThing* Weapon;
    FixtureCombatActionBase(FixtureThing* target, FixtureThing* weapon) : Target(target), Weapon(weapon) {}
    const FixtureThing* GetTarget() const { Log('T'); return Target; }
    FixtureThing* GetWeaponUsedInStrikeAction() const { Log('W'); return Weapon; }
    virtual FixtureCharString GetStrikeEventHelperName() const { Log('H'); return "strike_helper"; }
};

template<class From,class To> static To* GFDebugDynamicCast(From* from, To*)
{
    return static_cast<To*>(from);
}

namespace FixtureThingShot {
static FixtureThing* CreateShot(FixtureThing& shot, FixtureThing* shooter, FixtureThing* weapon,
    const FixtureThing* target, float accuracy, float a, float b,
    const FixtureDefPointer<const FixtureMeleeCombatAbilityDef>& ability, long index)
{
    Log('E');
    assert(&shot != shooter && shooter == weapon && target && !target->Dead);
    assert(shooter->BaseDef->Weapon->References == 1 && shot.BaseDef->Weapon->References == 1);
    assert(shot.BaseDef->References == 0 && shot.Physics.Orientation.Angle == 45);
    assert(accuracy == 38 && a == 1 && b == 0 && !ability.operator->());
    ++Shots;
    CreatedIndex = index;
    return Case->CreateSucceeds ? &shot : 0;
}

static FixtureThing* CreateShot(long index, FixtureThing* shooter, FixtureThing* weapon,
    const FixtureThing* target, float accuracy, float a, float b,
    const FixtureDefPointer<const FixtureMeleeCombatAbilityDef>& ability,
    const Fixture3DVector* pos, const FixtureRightHandedSet* rhs)
{
    Log('F');
    assert(shooter && !weapon && target && !target->Dead && target != shooter);
    assert(accuracy == 38 && a == 1 && b == 0 && !ability.operator->());
    assert(shooter->BaseDef->Weapon->References == 1);
    const bool dummy = Case->StrikeWeapon == 2 && Case->DummySucceeds;
    const bool helper = !dummy && Case->HelperSucceeds;
    assert(pos->X == (dummy ? 20 : helper ? 10 : 2));
    assert(pos->Y == (dummy ? 40 : helper ? 30 : 4));
    assert(pos->Z == (dummy ? 60 : helper ? 50 : 7));
    assert(rhs->Angle == (dummy ? 25 : 15));
    ++Shots;
    CreatedIndex = index;
    return Case->CreateSucceeds ? shooter : 0;
}
}

namespace FixtureCreatureAction {
struct FixtureActionEventShot {
    Creature& Source;
    explicit FixtureActionEventShot(Creature& source) : Source(source) {}
    Creature& GetThing() { return Source; }
    void Dispatch(FixtureCreatureActionBase& action);
};
}

void FixtureCreatureAction::FixtureActionEventShot::Dispatch(FixtureCreatureActionBase& action)
{
    FABLE_RETAIL_ASSERT(action.IsStrikeAction());
    if (action.IsStrikeAction()) {
        FixtureCombatActionBase& combat_action = *GFDebugDynamicCast(&action, static_cast<FixtureCombatActionBase*>(0));
        const FixtureThing* pTarget = combat_action.GetTarget();
        FixtureDefPointer<const FixtureWeaponDef> pweapon_def;
        if (IS_THING_ALIVE(pTarget) && GetThing().PeekBaseDef().GetSubDef(&pweapon_def)) {
            GetThing().MakeNoise("SIMPLE_NOISE");
            const FixtureTCCombat* ptc_combat;
            long index;
            if (GetThing().PeekTC(&ptc_combat) && ptc_combat->GetShotTypeOverride() > 0)
                index = ptc_combat->GetShotTypeOverride();
            else
                index = pweapon_def->Shot;
            if (index > 0) {
                long acc = GetThing().GetShotAccuracyPercentage();
                FixtureThing* pthing_carried = 0;
                FixtureTCCarrying* ptc_carrying;
                if (GetThing().GetTC(&ptc_carrying) &&
                    (pthing_carried = ptc_carrying->GetThingInPrimarySlot()) != 0) {
                    FixtureDefPointer<const FixtureWeaponDef> pcarried_thing_weapon_def;
                    if (pthing_carried->GetPBaseDef()->GetSubDef(&pcarried_thing_weapon_def) &&
                        pcarried_thing_weapon_def->Class == WC_PROJECTILE) {
                        FixtureRightHandedSet rhs(pthing_carried->GetPhysicsTC()->GetRHSet());
                        ptc_carrying->RemoveThing(*pthing_carried);
                        pthing_carried->GetPhysicsTC()->SetRHSet(rhs);
                        FixtureThingShot::CreateShot(*pthing_carried, &GetThing(), &GetThing(), pTarget, static_cast<float>(acc),
                            1.0f, 0.0f, FixtureDefPointer<const FixtureMeleeCombatAbilityDef>(), index);
                    } else {
                        pthing_carried = 0;
                    }
                }
                if (!pthing_carried) {
                    Fixture3DVector pos(GetThing().GetPhysicsTC()->GetPosition() + Fixture3DVector(0.0f, 0.0f, 1.0f));
                    FixtureRightHandedSet rhs(GetThing().GetPhysicsTC()->GetRHSet());
                    FixtureThing* pweapon = combat_action.GetWeaponUsedInStrikeAction();
                    FixtureCharString weapon_attachment_dummy("WEAPON_FOCUS_01");
                    FixtureCharString strike_event_helper_name = combat_action.GetStrikeEventHelperName();
                    if ((!IS_THING_ALIVE(pweapon) ||
                         !pweapon->GetTCGraphicAppearance()->GetDummyObject(weapon_attachment_dummy, 0, 0, pos, rhs, 1.0f)) &&
                        !GetThing().GetTCGraphicAppearance()->GetHelperPoint(strike_event_helper_name, 0, 0, pos, 1.0f)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    FixtureThing* pShot = FixtureThingShot::CreateShot(index, &GetThing(), 0, pTarget, static_cast<float>(acc),
                        1.0f, 0.0f, FixtureDefPointer<const FixtureMeleeCombatAbilityDef>(), &pos, &rhs);
                    static_cast<void>(pShot);
                }
            }
        }
    }
}

int main()
{
    const Scenario cases[] = {
        {"non_strike", false, 2, true, 12, false, 0, 0, 0, false, true, true, 0, "S"},
        {"null_target", true, 0, true, 12, false, 0, 0, 0, false, true, true, 0, "ST"},
        {"dead_target", true, 1, true, 12, false, 0, 0, 0, false, true, true, 0, "ST"},
        {"missing_weapon_def", true, 2, false, 12, false, 0, 0, 0, false, true, true, 0, "STD"},
        {"zero_default_index", true, 2, true, 0, false, 0, 0, 0, false, true, true, 0, "STDNC"},
        {"negative_default_index", true, 2, true, -3, true, -5, 0, 0, false, true, true, 0, "STDNC"},
        {"override_index", true, 2, true, 0, true, 19, 0, 0, false, true, true, 19, "STDNCAYGRWHJF"},
        {"zero_override_uses_default", true, 2, true, 12, true, 0, 1, 0, false, true, true, 12, "STDNCAYPGRWHJF"},
        {"carried_projectile", true, 2, true, 12, false, 0, 4, 2, true, true, true, 12, "STDNCAYPBdrXOE"},
        {"carried_projectile_creation_failure", true, 2, true, 12, true, 19, 4, 2, true, true, false, 19, "STDNCAYPBdrXOE"},
        {"carried_missing_def", true, 2, true, 12, false, 0, 2, 2, true, true, true, 12, "STDNCAYPBdGRWHMF"},
        {"carried_non_projectile", true, 2, true, 12, false, 0, 3, 2, true, true, true, 12, "STDNCAYPBdGRWHMF"},
        {"dead_strike_weapon", true, 2, true, 12, false, 0, 0, 1, true, true, true, 12, "STDNCAYGRWHJF"},
        {"dummy_fails_helper_succeeds", true, 2, true, 12, false, 0, 0, 2, false, true, true, 12, "STDNCAYGRWHMJF"},
        {"dummy_and_helper_fail", true, 2, true, 12, false, 0, 0, 2, false, false, false, 12, "STDNCAYGRWHMJF"}
    };
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        Case = &cases[i];
        Calls[0] = 0;
        CallCount = 0;
        CreatedIndex = 0;
        Shots = 0;
        FixtureWeaponDef source_weapon(WC_LIGHT, Case->ShotIndex);
        FixtureWeaponDef carried_weapon(Case->Carried == 4 ? WC_PROJECTILE : WC_LIGHT, 33);
        FixtureThingBaseDef source_def(Case->SourceDef ? &source_weapon : 0, false);
        FixtureThingBaseDef carried_def(Case->Carried >= 3 ? &carried_weapon : 0, true);
        FixtureThing carried, target, weapon;
        carried.BaseDef = &carried_def;
        carried.Physics.IsCarried = true;
        carried.Physics.Orientation.Angle = 45;
        target.Dead = Case->Target == 1;
        weapon.Dead = Case->StrikeWeapon == 1;
        Creature source(Case->Carried >= 2 ? &carried : 0);
        source.BaseDef = &source_def;
        FixtureCombatActionBase action(Case->Target ? &target : 0, Case->StrikeWeapon ? &weapon : 0);
        FixtureCreatureAction::FixtureActionEventShot event(source);
        event.Dispatch(action);
        assert(std::strcmp(Calls, Case->Expected) == 0 && CreatedIndex == Case->ExpectedIndex);
        assert(Shots == (CreatedIndex > 0 ? 1u : 0u));
        assert(source_weapon.References == 0 && carried_weapon.References == 0 && carried_def.References == 0);
    }
    std::puts("00871430_TEST PASS");
}