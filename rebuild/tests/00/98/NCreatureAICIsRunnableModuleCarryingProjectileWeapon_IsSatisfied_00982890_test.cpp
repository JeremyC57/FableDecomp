// Test: CIsRunnableModuleCarryingProjectileWeapon::IsSatisfied -- the creature def's RangedWeaponDef is positive (counted def pointer copied and released)
#include <cstdio>
class CThingCreatureDef { public: long RangedWeaponDef; };
struct CDefInfo { long RefCount; };
struct CDefPointer {
    const CThingCreatureDef* Def; CDefInfo* Info;
    CDefPointer(const CThingCreatureDef* d, CDefInfo* i) : Def(d), Info(i) { ++Info->RefCount; }
    CDefPointer(const CDefPointer& o) : Def(o.Def), Info(o.Info) { ++Info->RefCount; }
    ~CDefPointer() { --Info->RefCount; }
};
struct CCreature { CDefPointer PBaseDef; CDefPointer GetPBaseDef() const { return PBaseDef; } };
static bool IsSatisfied(const CCreature* c) { return c->GetPBaseDef().Def->RangedWeaponDef > 0; }
int main()
{
    CThingCreatureDef bow = { 3 }, none = { 0 }, neg = { -1 };
    CDefInfo i1 = { 0 }, i2 = { 0 }, i3 = { 0 };
    CCreature a = { CDefPointer(&bow, &i1) }, b = { CDefPointer(&none, &i2) }, c = { CDefPointer(&neg, &i3) };
    long before = i1.RefCount;
    if (!IsSatisfied(&a) || IsSatisfied(&b) || IsSatisfied(&c) || i1.RefCount != before) { std::printf("00982890_TEST FAIL\n"); return 1; }
    std::printf("00982890_TEST PASS\n");
    return 0;
}