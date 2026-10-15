// CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) -- the highest level an ability can be upgraded to: one less than its upgrade-cost count, clamped to 0..3 (0 for no ability / no def)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EDEA3, length 72 bytes.
// Callees: none. Reads the constant 3 through its .rdata home @ 0x01256188 (a `const long` bound to the
//          Dinkumware-style `min` reference).
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`): the clamped level lives in the dead
//          `ability` argument slot ([ebp+8]) because `min` takes it by reference.
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         Abilities (vector<pair<long, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160.
//         Retail CHeroAbilityDef keeps ExperienceCostsToUpgrade at +0x80 (PDB +0x8C: the def base and the two
//         vectors before it are 4 bytes narrower each).
// Shape: `level < 0` reset is a store into the address-taken local (`jns ; xor eax,eax ; mov [ebp+8],eax`),
//        then `Min(level, MAX_ABILITY_LEVEL)` = `max < level ? max : level` picks an address (`cmp eax,3 ; jg`)
//        and loads through it.

struct CLongVector {
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CHeroAbilityDef {
    unsigned char _pad_0x00[0x80];
    CLongVector   ExperienceCostsToUpgrade;       // +0x80 (PDB +0x8C)
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long                   Level;                 // +0x00
    const CHeroAbilityDef* PDef;                  // +0x04 CDefPointer::Object
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};

template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }

static const long MAX_ABILITY_LEVEL = 3;          // 0x01256188

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)

    long GetAbilityMaximumLevel(long ability);
};

long CTCInventoryAbilities_Retail::GetAbilityMaximumLevel(long ability)
{
    if (ability != 0) {
        const CHeroAbilityDef* def = Abilities[ability].PDef;
        if (def != 0) {
            long level = def->ExperienceCostsToUpgrade.size() - 1;
            if (level < 0)
                level = 0;
            return Min(level, MAX_ABILITY_LEVEL);
        }
    }
    return 0;
}
