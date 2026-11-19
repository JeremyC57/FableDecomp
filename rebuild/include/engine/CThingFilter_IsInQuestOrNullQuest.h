#ifndef FABLE_ENGINE_CTHINGFILTER_ISINQUESTORNULLQUEST_H
#define FABLE_ENGINE_CTHINGFILTER_ISINQUESTORNULLQUEST_H
// New header (hand4c 2026-11-19, row 0x00518340): CThingFilter_IsInQuestOrNullQuest, Ego_d.pdb class record (types 0x25F57 / field list
// 0x25F65: base std::unary_function<CThing,bool>, constructors (const CThingFilter_IsInQuestOrNullQuest&) compiler-generated and
// (const CCharString&), operator()(const CThing&) const, private CCharString QuestSectionName +0) and Ego_r.pdb's layout
// (struct_layouts_egor.tsv: sizeof 4, QuestSectionName +0). ??RCThingFilter_IsInQuestOrNullQuest@@QBE_NABVCThing@@@Z is out of line in
// retail (0x006A7C60, landed); the constructor's body is not recovered (declaration only).
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_IsInQuestOrNullQuest : public std::unary_function<CThing,bool> {
public:
    CThingFilter_IsInQuestOrNullQuest(const CCharString& quest_section_name);
    bool operator()(const CThing& thing) const;                          // 0x006A7C60
private:
    CCharString QuestSectionName;                                        // +0x00
};
#endif
