#include <cstdio>
static int g_destroyed = 0;
class CDefPointeeBase { public:
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) ++g_destroyed; }
};
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };
class CWeaponDef : public CDefPointeeBase { public:
    long Type; EWeaponClass Class; float AnimationSpeedVal; bool ProjectileWeaponAutoReload; bool NeedsLoading;
};
struct CWeaponDefPointer {
    CWeaponDef* Object;
    CWeaponDefPointer(CDefPointeeBase* o) : Object(static_cast<CWeaponDef*>(o)) { if (Object) Object->AddRef(); }
    ~CWeaponDefPointer() { Object->Release(); }
    CWeaponDef* operator->() const { return Object; }
};
class CThing { public: bool canUse; };
class CTCWeapon { public:
    CDefPointeeBase* WeaponDef_Object; bool ProjectileWeaponLoaded;
    bool IsUsableBy(const CThing& t) const { return t.canUse; }
    bool IsProjectileWeaponAutoLoading() const { CWeaponDefPointer def(WeaponDef_Object); return def->ProjectileWeaponAutoReload; }
};
#define FAIL do { std::printf("005da950_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CWeaponDef d; d.RefCount = 1; d.Type = 0; d.Class = WEAPON_CLASS_NONE; d.AnimationSpeedVal = 0; d.ProjectileWeaponAutoReload = false; d.NeedsLoading = false;
    CTCWeapon w; w.WeaponDef_Object = &d; w.ProjectileWeaponLoaded = false;
    d.ProjectileWeaponAutoReload = true; if (!w.IsProjectileWeaponAutoLoading() || d.RefCount != 1) FAIL;
    d.ProjectileWeaponAutoReload = false; if (w.IsProjectileWeaponAutoLoading()) FAIL;
    std::printf("005da950_TEST PASS\n");
    return 0;
}