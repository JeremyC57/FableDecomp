// CQuestManager::GetActiveQuestSectionNames(std::set<CCharString>& names) const -- the load sections of the active quests: each quest's name, then (while day / night is active) every name with "%" + "DayOnly" / "NightOnly" appended plus "NULL%DayOnly" / "NULL%NightOnly"
// Grade: DIFFER(856v832) -- frame alignment: VC7.1 builds an `and esp,-8` ebp frame (also with the prior's local models); bisected: present only when the ActiveQuests insert loop and a vector insert loop share the function, not caused by set::insert's reinterpret-cast pair copy (FABLE_RETAIL_STLP_SET_INSERT_RESULT probe). Body otherwise follows retail. 2 grades (one SRC_FAIL: missing CRegion forward declaration in the new opt-in).
// Leaf: ?GetActiveQuestSectionNames@CQuestManager@@QBEXAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B2CA0, 830 bytes (+ int3 padding to 0x004B2FE0), thiscall `ret 4`, frame 0x24, no EH frame.
// Identity: Ego_r.pdb ?GetActiveQuestSectionNames@CQuestManager@@QBEXAAV?$set@VCCharString@@...@std@@@Z; callers
//   CQuestManager::UninitializeDaySection 0x004B3260 / UninitializeNightSection 0x004B35E0 (landed), 0x004B3438, 0x004B362D,
//   CWorldMap::GetLoadableSections 0x004FF490. The catalog's "WorldMap_BuildLoadSectionsList" was descriptive.
// Callees: set<CCharString>'s _Rb_tree::insert_unique 0x004AC380 (pair<iterator,bool> through a hidden pointer),
//   vector<CCharString>::reserve 0x004BA7A0 and _M_insert_overflow 0x00433530, operator+(const CCharString&, const char*) 0x0099F600 /
//   (const CCharString&, const CCharString&) 0x0099F570 / (const char*, const CCharString&) 0x0099F690 (all __fastcall), CCharString
//   copy 0x0099EC30 / ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Globals: g_DayOnlySuffix 0x013BAC18 "DayOnly", g_NightOnlySuffix 0x013BAC1C "NightOnly" (descriptive, as the landed
//   UninitializeDaySection 0x004B3260); "%" 0x01239434, "NULL%" 0x01239438.
// Graded with FABLE_HDR_OVERLAY=$A/hdr (engine/CQuestManager.h FABLE_RETAIL_CQUESTMANAGER_SECTIONNAMES).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_BOASTUI
#define FABLE_RETAIL_CQUESTMANAGER_SECTIONNAMES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_VECTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CQuestManager.h"

extern CCharString g_DayOnlySuffix;      // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;    // 0x013BAC1C "NightOnly"

void CQuestManager::GetActiveQuestSectionNames(std::set<CCharString>& names) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator quest = ActiveQuests.begin(); quest != ActiveQuests.end(); ++quest)
        names.insert((*quest)->GetName());
    std::vector<CCharString> day_names;
    std::vector<CCharString> night_names;
    if (DayActive) {
        day_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            day_names.push_back(*it + "%" + g_DayOnlySuffix);
        day_names.push_back("NULL%" + g_DayOnlySuffix);
    }
    if (NightActive) {
        night_names.reserve(names.size() + 1);
        for (std::set<CCharString>::iterator it = names.begin(); it != names.end(); ++it)
            night_names.push_back(*it + "%" + g_NightOnlySuffix);
        night_names.push_back("NULL%" + g_NightOnlySuffix);
    }
    for (std::vector<CCharString>::iterator day = day_names.begin(); day != day_names.end(); ++day)
        names.insert(*day);
    for (std::vector<CCharString>::iterator night = night_names.begin(); night != night_names.end(); ++night)
        names.insert(*night);
}
