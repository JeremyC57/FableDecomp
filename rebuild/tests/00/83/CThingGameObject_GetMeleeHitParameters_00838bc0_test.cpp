// 00838bc0 behaviour test: CThingGameObject::GetMeleeHitParameters (final.cpp body over mock things and components)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector { float X, Y, Z; };
struct CThing;
struct CHitParameters {
    CThing* Hitter; CThing* Weapon; int Location; C3DVector Position, Direction;
    bool Localised, Directional, Flourish, KnockDown, Blockable, Decapitate, MoveBack, PlayHitResponseOverrideSet, PlayHitResponse;
    long Attribute; float Damage;
    void SetThingHitting(CThing& t) { Hitter = &t; }
    void SetWeapon(CThing* w) { Weapon = w; }
    void SetHitLocation(int* l) { Location = *l; }
    void AddDamageWithAttribute(long a, float d) { Attribute = a; Damage = d; }
};
struct CAction { bool Flourish, KnockDown, Blockable, Decapitate, UsesMult; float Mult; };
struct CTCWeaponModifier { float Mult; bool KnockDown; };
struct CExperience { float T, Low, High; };
struct CCharge { float Mult; C3DVector Hit; bool Decap; };
struct CWeaponTC { bool HasDamage; float Damage; long Attribute; };
struct CThing {
    bool Dead; unsigned ThingTypeFlags; CAction* CurrentAction; CTCWeaponModifier* Modifier; CExperience* Experience;
    float DamageMultiplierDef; CCharge* Charge; CWeaponTC* Weapon; float BaseMelee; bool HasCharge;
};
static void GetMeleeHitParameters(const CThing& self, int* location, CThing& hitter, CThing* weapon,
                                  const C3DVector& position, const C3DVector& direction, CHitParameters& params)
{
    params.SetThingHitting(hitter);
    if (weapon && !weapon->Dead)
        params.SetWeapon(weapon);
    if (location)
        params.SetHitLocation(location);
    params.Position = position;
    params.Localised = true;
    params.Direction = direction;
    params.Directional = true;
    float multiplier = 1.0f;
    if (hitter.ThingTypeFlags & 8) {
        CAction* action = hitter.CurrentAction;
        if (action) {
            if (action->Flourish) params.Flourish = true;
            if (action->KnockDown) params.KnockDown = true;
            if (!action->Blockable) params.Blockable = false;
            if (action->Decapitate) params.Decapitate = true;
            if (action->UsesMult) multiplier = action->Mult;
        }
    }
    float weaponMultiplier = 1.0f;
    CTCWeaponModifier* modifier = hitter.Modifier;
    if (weapon && !weapon->Dead && weapon != &hitter && modifier) {
        weaponMultiplier = modifier->Mult;
        params.MoveBack = false;
        params.Blockable = false;
        if (modifier->KnockDown) params.KnockDown = true;
    }
    if (hitter.Experience) {
        float t = hitter.Experience->T;
        float low = hitter.Experience->Low;
        multiplier *= low + (hitter.Experience->High - low) * t;
    }
    if (hitter.DamageMultiplierDef != 0.0f)
        multiplier *= hitter.DamageMultiplierDef;
    if (hitter.Charge) {
        multiplier = hitter.Charge->Mult * multiplier;
        params.Position = hitter.Charge->Hit;
        params.KnockDown = true;
        params.Decapitate = hitter.Charge->Decap;
    }
    if (self.HasCharge) {
        params.PlayHitResponseOverrideSet = true;
        params.PlayHitResponse = false;
    }
    long attribute; float damage;
    if (weapon && !weapon->Dead && weapon->Weapon && weapon->Weapon->HasDamage) {
        damage = weapon->Weapon->Damage;
        attribute = weapon->Weapon->Attribute;
    } else {
        attribute = 1;
        damage = hitter.BaseMelee;
    }
    params.AddDamageWithAttribute(attribute, damage * weaponMultiplier * multiplier);
}
int main()
{
    CAction act = { true, false, false, false, true, 2.0f };
    CTCWeaponModifier mod = { 1.5f, true };
    CExperience exp = { 0.5f, 1.0f, 3.0f };
    CWeaponTC wtc = { true, 10.0f, 3 };
    CThing weapon = { false, 0, 0, 0, 0, 0.0f, 0, &wtc, 0.0f, false };
    CThing hitter = { false, 8, &act, &mod, &exp, 0.5f, 0, 0, 4.0f, false };
    CThing self = { false, 0, 0, 0, 0, 0.0f, 0, 0, 0.0f, true };
    CHitParameters p = { 0 }; p.Blockable = true; p.MoveBack = true;
    C3DVector pos = { 1, 2, 3 }, dir = { 0, 1, 0 };
    int loc = 7;
    GetMeleeHitParameters(self, &loc, hitter, &weapon, pos, dir, p);
    CHECK(p.Hitter == &hitter && p.Weapon == &weapon && p.Location == 7 && p.Localised && p.Directional);
    CHECK(p.Flourish && p.KnockDown && !p.Blockable && !p.MoveBack && p.PlayHitResponseOverrideSet && !p.PlayHitResponse);
    // multiplier 2 * (1 + 2*0.5) * 0.5 = 2; damage 10 * 1.5 * 2 = 30
    CHECK(p.Attribute == 3 && p.Damage == 30.0f);
    CCharge ch = { 3.0f, { 9, 9, 9 }, true };
    CThing plain = { false, 0, 0, 0, 0, 0.0f, &ch, 0, 2.0f, false };
    CHitParameters q = { 0 };
    GetMeleeHitParameters(plain, 0, plain, 0, pos, dir, q);
    CHECK(q.Weapon == 0 && q.Location == 0 && q.Position.X == 9.0f && q.KnockDown && q.Decapitate);
    CHECK(q.Attribute == 1 && q.Damage == 6.0f && !q.PlayHitResponseOverrideSet);
    if (!g_fail) printf("00838bc0_TEST PASS\n");
    return g_fail;
}