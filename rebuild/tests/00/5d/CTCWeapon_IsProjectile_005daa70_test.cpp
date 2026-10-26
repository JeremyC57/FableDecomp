// test for 005daa70 CTCWeapon::IsProjectile
#include <stdio.h>
#include <string.h>
enum EWeaponType { WEAPON_TYPE_NONE, WEAPON_TYPE_PROJECTILE_A = 5, WEAPON_TYPE_PROJECTILE_B = 6 };
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CWeaponDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad08b[0x04];                         // +0x24
    EWeaponType   Type;                                  // +0x28
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    EWeaponType GetWeaponType() const { CDefPointer<CWeaponDef> def(WeaponDef); return def->Type; }
    bool IsProjectile() const;
};
bool CTCWeapon::IsProjectile() const
{
    EWeaponType type = GetWeaponType();
    if (type == WEAPON_TYPE_PROJECTILE_B || type == WEAPON_TYPE_PROJECTILE_A)
        return true;
    return false;
}

static int g_del;
CDefPointeeBase::~CDefPointeeBase() {}
void CDefPointeeBase::Delete() const { ++g_del; }
struct CWeaponDefT : CWeaponDef { };
int main()
{
    CWeaponDefT d; d.IVRefCount = 1; static double wbuf[4]; CTCWeapon& w = *(CTCWeapon*)wbuf; w.WeaponDef.Object = &d;
    int ok = 1;
    for (int t = 0; t < 9; ++t) { d.Type = (EWeaponType)t; ok = ok && w.IsProjectile() == (t == 5 || t == 6); }
    ok = ok && d.IVRefCount == 1 && g_del == 0;
    printf(ok ? "005daa70_TEST PASS\n" : "005daa70_TEST FAIL\n");
    return 0;
}