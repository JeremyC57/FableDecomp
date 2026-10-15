// CTCInventoryAbilities::AddAbilityToCorrectList(EHeroAbility, vector<CCharString>*, vector<CCharString>*) -- adds the ability's inventory thing name to the first list when the hero has the ability (level >= 0), else to the second
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EEFB7, length 59 bytes.
// Callees: CTCInventoryAbilities::GetAbilityInventoryThingName(EHeroAbility) @ 0x005EDD04 (CCharString by value
//          through the hidden pointer, homed in the dead `ability` slot [ebp+8]),
//          std::vector<CCharString>::push_back(const CCharString&) @ 0x0044BFF0, CCharString::~CCharString @ 0x0099EAE0.
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class Abilities
//         (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>, 8-byte entries) is at +0x160.
// Shape: an if/else whose two arms are the same call on different lists: VC hoists the argument pushes above
//        the branch, keeps one GetAbilityInventoryThingName call per arm, loads each arm's list into ecx and
//        merges the push_back and the temporary's destructor after the join.

struct CCharString {
    char* m_pData;
    ~CCharString();                               // 0x0099EAE0
};

struct CCharStringVector {                        // std::vector<CCharString>
    CCharString* first;
    CCharString* last;
    CCharString* end_of_storage;
    void push_back(const CCharString& value);     // 0x0044BFF0
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long  Level;                                  // +0x00 (-1 = not owned)
    void* PDef;                                   // +0x04
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryAbilities_Retail {
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)

    CCharString GetAbilityInventoryThingName(long ability);   // 0x005EDD04
    void AddAbilityToCorrectList(long ability, CCharStringVector* owned, CCharStringVector* notOwned);
};

void CTCInventoryAbilities_Retail::AddAbilityToCorrectList(long ability, CCharStringVector* owned, CCharStringVector* notOwned)
{
    if (Abilities[ability].Level >= 0)
        owned->push_back(GetAbilityInventoryThingName(ability));
    else
        notOwned->push_back(GetAbilityInventoryThingName(ability));
}
