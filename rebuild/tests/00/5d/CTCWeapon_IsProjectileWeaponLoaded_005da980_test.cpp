#include <cstdio>
static int g_destroyed = 0;
struct CDefPointeeBase {
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) ++g_destroyed; }
};
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };
struct CWeaponDef : CDefPointeeBase {
    long Type; EWeaponClass Class; float AnimationSpeedVal; bool ProjectileWeaponAutoReload; bool NeedsLoading;
};
struct CWeaponDefPointer {
    CWeaponDef* Object;
    CWeaponDefPointer(CDefPointeeBase* o) : Object(static_cast<CWeaponDef*>(o)) { if (Object) Object->AddRef(); }
    ~CWeaponDefPointer() { Object->Release(); }
    CWeaponDef* operator->() const { return Object; }
};
struct CThing { bool canUse; };
struct CTCWeapon {
    CDefPointeeBase* WeaponDef_Object; bool ProjectileWeaponLoaded;
    bool IsUsableBy(const CThing& t) const { return t.canUse; }
    bool IsProjectileWeaponLoaded() const { bool n; { CWeaponDefPointer def(WeaponDef_Object); n = def->NeedsLoading; } if (n) return ProjectileWeaponLoaded; return true; }
};
#define FAIL do { std::printf("005da980_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CWeaponDef d; d.RefCount = 1; d.Type = 0; d.Class = WEAPON_CLASS_NONE; d.AnimationSpeedVal = 0; d.ProjectileWeaponAutoReload = false; d.NeedsLoading = false;
    CTCWeapon w; w.WeaponDef_Object = &d; w.ProjectileWeaponLoaded = false;
    d.NeedsLoading = false; w.ProjectileWeaponLoaded = false; if (!w.IsProjectileWeaponLoaded()) FAIL;
    d.NeedsLoading = true; if (w.IsProjectileWeaponLoaded() || d.RefCount != 1) FAIL;
    w.ProjectileWeaponLoaded = true; if (!w.IsProjectileWeaponLoaded()) FAIL;
    std::printf("005da980_TEST PASS\n");
    return 0;
}