// NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs() -- the bandit creature def for the population sim: fill a list by the hero's rough experience level (grunt strings from ChooseRandomBanditGruntString plus archer / leader defs per level band), take entry BanditCreatureGeneratorIndex % size, advance the index and return it
// Grade: DIFFER(2040v2016) -- frequency + alias residue: with the default label written first, VC7.1 expands STLport push_back at the five default-arm sites as retail does, but it keeps the freshly zeroed vector words across the virtual GetHeroRoughExperienceLevel call (the first default site folds to a direct _M_insert_overflow call, a zero lives in ebx, _M_finish in edi, one more callee-saved push), where retail reloads _M_start/_M_finish/_M_end_of_storage from the frame at every site and in the tail.
// Next idea: find the source shape that makes the vector's words unknown at the switch (retail treats them as clobbered by the virtual call): e.g. the list built by a reference to the vector taken through a call or a member, or the default arm reached from a second path; check other retail functions with an inline push_back right after a virtual call for the same reload pattern.
// Leaf: ?ChooseRandomBanditFromDefs@CV_RandomPopulationSimScript@NScript@@QAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC32F0, 2016 bytes, thiscall `ret 4` (0x00EC3AA8), then the 9-entry jump table 0x00EC3AAC.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ChooseRandomBanditFromDefs@CV_RandomPopulationSimScript@NScript@@QAE?AVCCharString@@XZ;
//   it sits between ChooseRandomGuardFromDefs 0x00EC3230 and ChooseRandomBanditGruntString 0x00EC3AD0 (its callee, PDB-named), bumps
//   BanditCreatureGeneratorIndex +0x6C (Ego_r +0x80, the class is 0x14 smaller in retail), CREATURE_BANDIT_* literals.
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, copy 0x0099EC30, ~CCharString 0x0099EAE0,
//   operator=(const CCharString&) 0x0099EFB0, ChooseRandomBanditGruntString 0x00EC3AD0, vector<CCharString>::push_back 0x0044BFF0
//   (out of line) / _M_insert_overflow 0x00433530, free 0x00BFEA14; Game slot 175 GetHeroRoughExperienceLevel.
// Shape: cases 0..3 level-1 grunts (+ archers, + leader), 4..7 level 2, 8 and default level 3; each case's last temporary dies in the
//   shared tail; the index is unsigned (`div`).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_RandomPopulationSimScript {
public:
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                     // +0x40
    unsigned char _pad44[0x6c - 0x44];
    unsigned long BanditCreatureGeneratorIndex;     // +0x6c (PDB +0x80)
    unsigned long VillagerCreatureGeneratorIndex;   // +0x70
    unsigned long GuardCreatureGeneratorIndex;      // +0x74
    CCharString ChooseRandomBanditGruntString(long level);
    CCharString ChooseRandomBanditFromDefs();
};
}
CCharString NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs()
{
    CCharString bandit;
    std::vector<CCharString> defs;
    switch (Game->GetHeroRoughExperienceLevel()) {
    default:
        defs.push_back(ChooseRandomBanditGruntString(3));
        defs.push_back(ChooseRandomBanditGruntString(3));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        defs.push_back("CREATURE_BANDIT_LEADER_LEVEL3");
        break;
    case 0:
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        break;
    case 1:
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        break;
    case 2:
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        break;
    case 3:
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back(ChooseRandomBanditGruntString(1));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        defs.push_back("CREATURE_BANDIT_LEADER_LEVEL1");
        break;
    case 4:
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        break;
    case 5:
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        break;
    case 6:
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        break;
    case 7:
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back(ChooseRandomBanditGruntString(2));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        defs.push_back("CREATURE_BANDIT_LEADER_LEVEL2");
        break;
    case 8:
        defs.push_back(ChooseRandomBanditGruntString(3));
        defs.push_back(ChooseRandomBanditGruntString(3));
        defs.push_back(ChooseRandomBanditGruntString(3));
        defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        break;
    }
    bandit = defs[BanditCreatureGeneratorIndex % defs.size()];
    ++BanditCreatureGeneratorIndex;
    return bandit;
}
