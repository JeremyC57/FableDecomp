// 004b2ca0 CQuestManager::GetActiveQuestSectionNames
// Flags:
#include "rebuild_abi.h"
#include "fable_compiler.h"
#define FABLE_RETAIL_QUEST_SECTION_OVERFLOW_PASS29
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
// Leaf: ?GetActiveQuestSectionNames@CQuestManager@@QBEXAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CQUESTMANAGER_FAILUREBOOKKEEP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_SET_RANGE
#define FABLE_RETAIL_CQUESTMANAGER_SECTIONNAMES_PASS29
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_VECTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CQuestManager.h"

#include "engine/CTCDayOrNightOnlySupport.h"

void CQuestManager::GetActiveQuestSectionNames(std::set<CCharString>& names) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator quest = ActiveQuests.begin(); quest != ActiveQuests.end(); ++quest)
    {
        CActiveQuest& active_quest=**quest;
        names.insert(active_quest.GetSerialisationSectionName());
    }
    std::vector<CCharString> day_names;
    std::vector<CCharString> night_names;
    if (DayActive) {
        day_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            day_names.push_back(*it + "%" + CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX);
        day_names.push_back("NULL%" + CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX);
    }
    if (NightActive) {
        night_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            night_names.push_back(*it + "%" + CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX);
        night_names.push_back("NULL%" + CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX);
    }
    names.insert(day_names.begin(), day_names.end());
    names.insert(night_names.begin(), night_names.end());
}