// CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs() -- the guard creature def name for the population sim ("CREATURE_BS_GUARD"), advancing the guard generator index; returns the CCharString by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC3230, length 62 bytes.
// Leaf: ChooseRandomGuardFromDefs
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::operator=(const char*) @ 0x0099EFE0,
// CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0.
// Identity: the manifest label "COpinionSourceDef::COpinionSourceDef" is propagated and rejected (the body
// returns a string through the hidden pointer and never stores a vtable). It sits between
// CV_RandomPopulationSimScript::ChooseRandomVillagerFromDefs (0x00EC3130, which bumps +0x70 the same way)
// and GetCurrentPopulationLevel / ChooseRandomBanditFromDefs, and both callers (0x00EC2C7A, 0x00EC2F9E) feed
// the result to the creature-spawn call of the population sim; the leaf name is inferred from those
// siblings. Ego_r.pdb CV_RandomPopulationSimScript: Bandit/Villager/GuardCreatureGeneratorIndex at
// +0x80/+0x84/+0x88, 0x14 lower in retail (GetCurrentPopulationLevel reads CurrentPopulationScale at +0x48
// against the PDB's +0x5c), so the guard index is +0x74.
// Shape: a default-constructed local assigned from the literal, the member increment, then the copy
// into the hidden result and the local's destruction (VC7.1 has no NRVO; no EH frame in this module).
struct CCharString {
    char* m_pString;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const char* text);       // 0x0099EFE0
};

struct CV_RandomPopulationSimScript {
    unsigned char _base[0x6c];                      // +0x00 script base, population scales, region
    unsigned long BanditCreatureGeneratorIndex;     // +0x6c (by the PDB order; not touched here)
    unsigned long VillagerCreatureGeneratorIndex;   // +0x70
    unsigned long GuardCreatureGeneratorIndex;      // +0x74
    CCharString ChooseRandomGuardFromDefs();
};

CCharString CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs()
{
    CCharString defName;
    defName = "CREATURE_BS_GUARD";
    ++GuardCreatureGeneratorIndex;
    return defName;
}
