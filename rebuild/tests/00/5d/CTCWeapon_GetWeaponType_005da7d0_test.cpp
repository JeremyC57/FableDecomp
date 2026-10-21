// behaviour test for 0x005DA7D0
#include <stdio.h>
#include <string.h>
static int g_deletes = 0;
enum EWeaponType { WEAPON_TYPE_NONE };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
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
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    EWeaponType GetWeaponType() const;
};
EWeaponType CTCWeapon::GetWeaponType() const
{
    CDefPointer<CWeaponDef> def(WeaponDef);
    return def->Type;
}

int main()
{
    CWeaponDef def; def.IVRefCount = 1; def.Type = (EWeaponType)0x48;
    static long buf[8]; CTCWeapon& tc = *(CTCWeapon*)buf; tc.WeaponDef.Object = &def;
    int ok = tc.GetWeaponType() == 0x48 && def.IVRefCount == 1 && g_deletes == 0;
    def.IVRefCount = 0; tc.WeaponDef.Object = 0;
    printf(ok ? "005da7d0_TEST PASS\n" : "005da7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}