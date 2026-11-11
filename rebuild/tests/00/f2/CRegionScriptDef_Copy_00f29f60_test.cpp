// Behaviour test for CRegionScriptDef::Copy (0x00F29F60): the virtual clone hook assigns the source def's fields over this def.
#include <stdio.h>
class CDefClassBase { public:
    int baseField;
    virtual ~CDefClassBase() { }
    virtual void Copy(const CDefClassBase* def) = 0;
};
class CRegionScriptDef : public CDefClassBase { public:
    long RandomVillagerMax, RandomGuardMax, RandomBanditMax, RegionDangerLevel;
    virtual void Copy(const CDefClassBase* def);
};
void CRegionScriptDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CRegionScriptDef*>(def);
}
int main(void)
{
    CRegionScriptDef a, b;
    a.baseField = 7; a.RandomVillagerMax = 1; a.RandomGuardMax = 2; a.RandomBanditMax = 3; a.RegionDangerLevel = 4;
    b.baseField = 0; b.RandomVillagerMax = b.RandomGuardMax = b.RandomBanditMax = b.RegionDangerLevel = -1;
    CDefClassBase* p = &b;
    p->Copy(&a);
    if (b.baseField != 7 || b.RandomVillagerMax != 1 || b.RandomGuardMax != 2 || b.RandomBanditMax != 3 || b.RegionDangerLevel != 4) {
        printf("00f29f60_TEST FAIL\n"); return 1;
    }
    printf("00f29f60_TEST PASS\n");
    return 0;
}