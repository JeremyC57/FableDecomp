// CV_FisticuffsClubScript::GetFighterName() -- the definition name of the current region's punch-club member ("CREATURE_BS/KHG/OV/BC_FISTICUFFS_MEMBER"), "An error" for an unknown region, returned by value
// Leaf: GetFighterName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E848E0, length 133 bytes (jump table at 0x00E84968 inside the row), `ret 4`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString copy ctor @ 0x0099EC30,
// ~CCharString @ 0x0099EAE0. Literals: "An error" 0x012EB2AC, "CREATURE_BS_FISTICUFFS_MEMBER" 0x012EB28C,
// "CREATURE_KHG_FISTICUFFS_MEMBER" 0x012EB26C, "CREATURE_OV_FISTICUFFS_MEMBER" 0x012EB24C,
// "CREATURE_BC_FISTICUFFS_MEMBER" 0x012EB22C.
// Layout: RelevantRegion at retail +0x48 (Ego_r.pdb - 0x14).
// Shape: an unused "An error" local from an inline by-value helper (VC7.1 has no NRVO: build, copy, destroy) heads the body; the cases in source
// order 1, 4, 2, 3 share one result-construction tail.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
struct CV_FisticuffsClubScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    unsigned char _pad4c[0x88 - 0x4c];
    long FightCount;                               // +0x88
    bool EverybodyAttack;                          // +0x8c
    bool LostWhileOutside;                         // +0x8d
    bool FisticuffsActive;                         // +0x8e
    long AllowedHits;                              // +0x90
    CCharString GetFighterName();
};
inline CCharString MakeString(const char* text)    // by value without NRVO: build, copy out, destroy
{
    CCharString result(text);
    return result;
}
CCharString CV_FisticuffsClubScript::GetFighterName()
{
    CCharString name = MakeString("An error");
    switch (RelevantRegion)
    {
    case 1:
        return CCharString("CREATURE_BS_FISTICUFFS_MEMBER");
    case 4:
        return CCharString("CREATURE_KHG_FISTICUFFS_MEMBER");
    case 2:
        return CCharString("CREATURE_OV_FISTICUFFS_MEMBER");
    case 3:
        return CCharString("CREATURE_BC_FISTICUFFS_MEMBER");
    }
    return CCharString("An error");
}