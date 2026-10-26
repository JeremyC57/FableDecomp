// CV_RandomPopulationSimScript::ChooseRandomVillagerFromDefs() -- a random villager creature def name for the population sim: 40% a stocked trader (one of four), 10% a tattooist (one of three), 10% a barber (one of two), otherwise the plain trader; advances the villager generator index; returns the CCharString by value
// Leaf: ChooseRandomVillagerFromDefs
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC3130, 236 bytes + a four-entry jump table (row 256 with padding), `ret 4`.
// Callees: CCharString() 0x0099E4B0, rand 0x00BFEB16 (MSVCR71 import), CCharString::operator=(const char*) 0x0099EFE0,
//   CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0.
// Strings: "CREATURE_TRADER_STOCKED_01".."_04" 0x012F153C/1520/1504/14E8, "CREATURE_TRADER_STOCKED_TATTOOIST_01".."_03"
//   0x012F14C0/1498/1470, "CREATURE_TRADER_STOCKED_BARBER_01"/"_02" 0x012F144C/1428, "CREATURE_TRADER_01" 0x012E15A4.
// Shape: the landed sibling ChooseRandomGuardFromDefs (0x00EC3230): a default-constructed local assigned from the
//   chosen literal, the member increment, then the copy into the hidden result. The `rand() % 10` switch (cases 0 and
//   1 share the stocked-trader block) has each case's name table as a block-scoped array (all three share the same
//   frame words) and VC tail-merges every `defName = table[rand() % n]` into one operator= call.
// Layout: VillagerCreatureGeneratorIndex +0x70 (Ego_r.pdb +0x84, 0x14 lower in retail; landed sibling).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    char* m_pString;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const char* text);       // 0x0099EFE0
};
extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)

namespace NScript {
class CV_RandomPopulationSimScript {
public:
    unsigned char _base[0x6c];                      // +0x00 script base, population scales, region
    unsigned long BanditCreatureGeneratorIndex;     // +0x6c
    unsigned long VillagerCreatureGeneratorIndex;   // +0x70
    unsigned long GuardCreatureGeneratorIndex;      // +0x74
    CCharString ChooseRandomVillagerFromDefs();
};
} // namespace NScript
using NScript::CV_RandomPopulationSimScript;

CCharString NScript::CV_RandomPopulationSimScript::ChooseRandomVillagerFromDefs()
{
    CCharString defName;
    switch (rand() % 10) {
    case 0:
    case 1: {
        const char* names[4] = { "CREATURE_TRADER_STOCKED_01", "CREATURE_TRADER_STOCKED_02",
                                 "CREATURE_TRADER_STOCKED_03", "CREATURE_TRADER_STOCKED_04" };
        defName = names[rand() % 4];
        break;
    }
    case 2: {
        const char* names[3] = { "CREATURE_TRADER_STOCKED_TATTOOIST_01", "CREATURE_TRADER_STOCKED_TATTOOIST_02",
                                 "CREATURE_TRADER_STOCKED_TATTOOIST_03" };
        defName = names[rand() % 3];
        break;
    }
    case 3: {
        const char* names[2] = { "CREATURE_TRADER_STOCKED_BARBER_01", "CREATURE_TRADER_STOCKED_BARBER_02" };
        defName = names[rand() % 2];
        break;
    }
    default:
        defName = "CREATURE_TRADER_01";
        break;
    }
    ++VillagerCreatureGeneratorIndex;
    return defName;
}