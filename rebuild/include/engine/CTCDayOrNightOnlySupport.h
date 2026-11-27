#ifdef FABLE_RETAIL_CQUESTMANAGER_SECTIONNAMES_PASS29
#ifndef FABLE_ENGINE_CTCDAYORNIGHTONLY_SUPPORT_H
#define FABLE_ENGINE_CTCDAYORNIGHTONLY_SUPPORT_H
#include "engine/CTCBase.h"
#include "fable_string.h"
#ifndef FABLE_RETAIL_QUEST_SECTION_OVERFLOW_PASS29
enum EDayNightOrBoth;
#endif
// Original component at native 16 bytes. The public const data objects are
// bound at 013BAC18/013BAC1C by both relocated dynamic initializers.
class CTCDayOrNightOnlySupport : public CTCBase {
public:
#ifdef FABLE_RETAIL_QUEST_SECTION_OVERFLOW_PASS29
    enum EDayNightOrBoth { BOTH_DAY_AND_NIGHT, DAY_ONLY, NIGHT_ONLY };
#endif
    static const CCharString DAY_ONLY_SUFFIX;
    static const CCharString NIGHT_ONLY_SUFFIX;
private:
    EDayNightOrBoth DayNightOrBoth;
};
FABLE_STATIC_ASSERT_AT(14,sizeof(CTCDayOrNightOnlySupport)==16);
#endif
#endif
