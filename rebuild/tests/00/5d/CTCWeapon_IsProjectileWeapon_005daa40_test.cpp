// test for 005daa40 CTCWeapon::IsProjectileWeapon
#include <stdio.h>
enum EWeaponType { WEAPON_TYPE_NONE };
enum EWeaponClass { WEAPON_CLASS_NONE, WEAPON_CLASS_1, WEAPON_CLASS_2, WEAPON_CLASS_PROJECTILE };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad08b[0x04];                         // +0x24
    EWeaponType   Type;                                  // +0x28
    EWeaponClass  Class;                                 // +0x2c
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    static bool IsProjectileWeaponClass(EWeaponClass weapon_class) { return weapon_class == WEAPON_CLASS_PROJECTILE; }
    EWeaponClass GetWeaponClass() const { CDefPointer<CWeaponDef> def(WeaponDef); return def->Class; }
    bool IsProjectileWeapon() const;
};
bool CTCWeapon::IsProjectileWeapon() const
{
    return IsProjectileWeaponClass(GetWeaponClass());
}

static int g_deletes;
CDefPointeeBase::~CDefPointeeBase() {}
void CDefPointeeBase::Delete() const { ++g_deletes; }
int main()
{
    CWeaponDef def; def.IVRefCount = 1; def.Class = WEAPON_CLASS_PROJECTILE;
    static long buf[8]; CTCWeapon& tc = *(CTCWeapon*)buf; tc.WeaponDef.Object = &def;
    int ok = tc.IsProjectileWeapon() == true && def.IVRefCount == 1 && g_deletes == 0;
    def.Class = WEAPON_CLASS_2; ok &= tc.IsProjectileWeapon() == false;
    def.IVRefCount = 0; def.Class = WEAPON_CLASS_PROJECTILE; ok &= tc.IsProjectileWeapon() == true && g_deletes == 1;
    ok &= (char*)&def.Class - (char*)(CDefPointeeBase*)&def == 0x2c;
    tc.WeaponDef.Object = 0;
    printf(ok ? "005daa40_TEST PASS\n" : "005daa40_TEST FAIL\n");
    return ok ? 0 : 1;
}