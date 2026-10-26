// Test for 0x006623F0 CThingCreatureBase::GetMeleeWeaponDef: returns the def's PrimaryMeleeWeaponDef.
#include <stdio.h>
class CThingCreatureDef {
public:
    unsigned char _pad00[0xD8];
    long          PrimaryMeleeWeaponDef;     // +0xD8 (Ego_r +0xE0)
    long          SecondaryMeleeWeaponDef;   // +0xDC
    long          RangedWeaponDef;           // +0xE0 (Ego_r +0xE8)
};

template <class T> class CDefPointer {
public:
    T* m_p;
    T* operator->() const { return m_p; }
};

class CThingCreatureBase {
public:
    unsigned char _pad00[0xE0];
    CDefPointer<const CThingCreatureDef> PDef;   // +0xE0 (Ego_r +0xFC)
    long GetMeleeWeaponDef() const;
};

int main()
{
    CThingCreatureDef d; d.PrimaryMeleeWeaponDef = 11; d.SecondaryMeleeWeaponDef = 22; d.RangedWeaponDef = 33;
    CThingCreatureBase c; c.PDef.m_p = &d;
    if ((char*)&c.PDef - (char*)&c != 0xE0 || (char*)&d.PrimaryMeleeWeaponDef - (char*)&d != 0xD8) { printf("006623f0_TEST FAIL layout\n"); return 1; }
    if (c.GetMeleeWeaponDef() != d.PrimaryMeleeWeaponDef) { printf("006623f0_TEST FAIL\n"); return 1; }
    d.PrimaryMeleeWeaponDef = -1;
    if (c.GetMeleeWeaponDef() != -1) { printf("006623f0_TEST FAIL reread\n"); return 1; }
    printf("006623f0_TEST PASS\n");
    return 0;
}
