// behaviour test for 0x00453F8E: CCombatAbilityUseProjectileWeaponDef::Copy forwards to CCombatAbilityUseProjectileWeaponDef::operator= with this and the source object
// NF386 (NF324 Q2 form, NF337 gen.py): the row now defines the PDB member CCombatAbilityUseProjectileWeaponDef::Copy (?Copy@CCombatAbilityUseProjectileWeaponDef@@UAEXPBVCDefClassBase@@@Z).
// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never
// constructed (no vftable, no RTTI). The callee operator= (??4CCombatAbilityUseProjectileWeaponDef@@QAEAAV0@ABV0@@Z, out of line in the row) is stubbed as a member here.
#include <cstdio>

class CDefClassBase;
class CCombatAbilityUseProjectileWeaponDef { public:
    virtual void Copy(const CDefClassBase* other);
    CCombatAbilityUseProjectileWeaponDef& operator=(const CCombatAbilityUseProjectileWeaponDef& other);
};

static int g_calls = 0;
static const void* g_this = 0;
static const void* g_other = 0;
CCombatAbilityUseProjectileWeaponDef& CCombatAbilityUseProjectileWeaponDef::operator=(const CCombatAbilityUseProjectileWeaponDef& other)
{
    ++g_calls; g_this = this; g_other = &other;
    return *this;
}

static unsigned long g_dst[4];
static unsigned long g_src[4];

int main()
{
    ((CCombatAbilityUseProjectileWeaponDef*)(g_dst))->CCombatAbilityUseProjectileWeaponDef::Copy((const CDefClassBase*)(g_src));
    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src) {
        std::printf("CCombatAbilityUseProjectileWeaponDef_00453f8e_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL_00453f8e calls=%d\n", g_calls);
    return 1;
}
