// CV_FisticuffsClubScript::GetChampionName() -- the definition name of the current region's punch-club champion (Bowerstone, Knothole Glade chief, Oakvale, Bandit Camp), "An error" for an unknown region, returned by value
// Leaf: GetChampionName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E84980, length 133 bytes (jump table at 0x00E84A08 inside the row), `ret 4`.
// The catalog names both 0x00E848E0 and this body "GetFighterName"; this one returns the champions, so it is named
// descriptively (the real PDB name is not known).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString copy ctor @ 0x0099EC30,
// ~CCharString @ 0x0099EAE0. Literals: "An error" 0x012EB2AC, "CREATURE_BS_PUNCH_CLUB_CHAMPION_01" 0x012EB314,
// "CREATURE_KG_CHIEF" 0x012EB300, "CREATURE_OV_PUNCH_CLUB_CHAMPION_01" 0x012EB2DC,
// "CREATURE_BC_PUNCH_CLUB_CHAMPION_01" 0x012EB2B8. Twin of GetFighterName 0x00E848E0.
// Layout: RelevantRegion at retail +0x48 (Ego_r.pdb - 0x14).
// Shape: an unused "An error" local from an inline by-value helper (VC7.1 has no NRVO: build, copy, destroy) heads the body; the cases in source
// order 1, 4, 2, 3 share one result-construction tail.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
namespace NScript {
class CV_FisticuffsClubScript {
public:
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    unsigned char _pad4c[0x88 - 0x4c];
    long FightCount;                               // +0x88
    bool EverybodyAttack;                          // +0x8c
    bool LostWhileOutside;                         // +0x8d
    bool FisticuffsActive;                         // +0x8e
    long AllowedHits;                              // +0x90
    CCharString GetChampionName();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
inline CCharString MakeString(const char* text)    // by value without NRVO: build, copy out, destroy
{
    CCharString result(text);
    return result;
}
CCharString NScript::CV_FisticuffsClubScript::GetChampionName()
{
    CCharString name = MakeString("An error");
    switch (RelevantRegion)
    {
    case 1:
        return CCharString("CREATURE_BS_PUNCH_CLUB_CHAMPION_01");
    case 4:
        return CCharString("CREATURE_KG_CHIEF");
    case 2:
        return CCharString("CREATURE_OV_PUNCH_CLUB_CHAMPION_01");
    case 3:
        return CCharString("CREATURE_BC_PUNCH_CLUB_CHAMPION_01");
    }
    return CCharString("An error");
}