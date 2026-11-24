// NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs() -- Choose the next grunt, archer or leader definition from the hero experience band.
// Leaf: ?ChooseRandomBanditFromDefs@CV_RandomPopulationSimScript@NScript@@QAE?AVCCharString@@XZ
// Flags: default
// Grade: RELOCATION_MATCH
// Inferred: the returned string is copied before the vector destructor, as seen in the donor.

#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_RANDOMPOPULATIONSIMSCRIPT_BANDITDEFS
#include "engine/CCountedPointer.h"
#include "engine/NScript_CV_RandomPopulationSimScript.h"
#include "engine/CGameScriptInterfaceBase.h"
CCharString NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs()
{
    CCharString bandit_def_name;
    std::vector<CCharString> families;
    long experience = Game->GetHeroRoughExperienceLevel();
    switch (experience) {
    case 0:
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        break;
    case 1:
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        break;
    case 2:
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        break;
    case 3:
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back(ChooseRandomBanditGruntString(1));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
        families.push_back("CREATURE_BANDIT_LEADER_LEVEL1");
        break;
    case 4:
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        break;
    case 5:
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        break;
    case 6:
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        break;
    case 7:
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back(ChooseRandomBanditGruntString(2));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
        families.push_back("CREATURE_BANDIT_LEADER_LEVEL2");
        break;
    case 8:
        families.push_back(ChooseRandomBanditGruntString(3));
        families.push_back(ChooseRandomBanditGruntString(3));
        families.push_back(ChooseRandomBanditGruntString(3));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        break;
    default:
        families.push_back(ChooseRandomBanditGruntString(3));
        families.push_back(ChooseRandomBanditGruntString(3));
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        families.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
        families.push_back("CREATURE_BANDIT_LEADER_LEVEL3");
        break;
    }
    bandit_def_name = families[BanditCreatureGeneratorIndex % families.size()];
    ++BanditCreatureGeneratorIndex;
    return bandit_def_name;
}