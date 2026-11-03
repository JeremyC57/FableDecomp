#ifndef FABLE_ENGINE_CTHINGFILTER_ISINQUESTORNULLQUEST_H
#define FABLE_ENGINE_CTHINGFILTER_ISINQUESTORNULLQUEST_H
// Original Ego_r TPI: unary_function<CThing,bool> base, CCharString QuestSectionName (size 4).
// operator() is out of line: Ego_r 0x005A3900 ??RCThingFilter_IsInQuestOrNullQuest@@QBE_NABVCThing@@@Z.
// (2026-11-03, chunk 719 row 0x00509A20, agent719c.)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingFilter_IsInQuestOrNullQuest : public std::unary_function<CThing,bool> {
public:
    CThingFilter_IsInQuestOrNullQuest(const CCharString& name):QuestSectionName(name){}
    bool operator()(const CThing& thing) const;
protected:
    CCharString QuestSectionName;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsInQuestOrNullQuest)==4);
#endif
