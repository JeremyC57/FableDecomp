// Test for 0x00662400 CThingCreatureBase::GetRangedWeaponDef: returns the def's RangedWeaponDef.
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
    long GetRangedWeaponDef() const;
};

int main()
{
    CThingCreatureDef d; d.PrimaryMeleeWeaponDef = 11; d.SecondaryMeleeWeaponDef = 22; d.RangedWeaponDef = 33;
    CThingCreatureBase c; c.PDef.m_p = &d;
    if ((char*)&c.PDef - (char*)&c != 0xE0 || (char*)&d.RangedWeaponDef - (char*)&d != 0xE0) { printf("00662400_TEST FAIL layout\n"); return 1; }
    if (c.GetRangedWeaponDef() != d.RangedWeaponDef) { printf("00662400_TEST FAIL\n"); return 1; }
    d.RangedWeaponDef = -1;
    if (c.GetRangedWeaponDef() != -1) { printf("00662400_TEST FAIL reread\n"); return 1; }
    printf("00662400_TEST PASS\n");
    return 0;
}
