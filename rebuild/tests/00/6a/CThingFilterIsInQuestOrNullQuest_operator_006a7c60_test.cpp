// Behaviour test for 006a7c60: CThingFilter_IsInQuestOrNullQuest -- the thing's section equals the quest section, "NULL" or "" (model copy).
#include <cstdio>
#include <cstring>
struct CData { const char* PData; unsigned long StringLength; bool operator==(const char* t) const { return std::strcmp(PData, t) == 0; } };
struct CCharString {
    CData* PStringData;
    bool operator==(const CCharString& o) const
    {
        if (PStringData == o.PStringData) return true;
        if (PStringData && o.PStringData && PStringData->StringLength == o.PStringData->StringLength)
            return std::strcmp(PStringData->PData, o.PStringData->PData) ? false : true;
        return false;
    }
    bool operator==(const char* t) const { if (PStringData == 0) return std::strcmp(t, "") == 0; return *PStringData == t; }
};
struct CThing { CCharString SerialisationSectionName; };
struct CThingFilter_IsInQuestOrNullQuest {
    CCharString QuestSectionName;
    bool operator()(const CThing& t) const
    { return t.SerialisationSectionName == QuestSectionName || t.SerialisationSectionName == "NULL" || t.SerialisationSectionName == ""; }
};
int main()
{
    CData q1 = {"Q_Arena", 7}, q2 = {"Q_Arena", 7}, other = {"Q_Guild", 7}, nul = {"NULL", 4}, empty = {"", 0}, longer = {"Q_ArenaX", 8};
    CThingFilter_IsInQuestOrNullQuest f; f.QuestSectionName.PStringData = &q1;
    CThing t; int fail = 0;
    t.SerialisationSectionName.PStringData = &q1;    if (!f(t)) fail = 1;   // same block
    t.SerialisationSectionName.PStringData = &q2;    if (!f(t)) fail = 1;   // equal text
    t.SerialisationSectionName.PStringData = &other; if (f(t)) fail = 1;    // another quest
    t.SerialisationSectionName.PStringData = &longer; if (f(t)) fail = 1;
    t.SerialisationSectionName.PStringData = &nul;   if (!f(t)) fail = 1;   // the null quest
    t.SerialisationSectionName.PStringData = &empty; if (!f(t)) fail = 1;
    t.SerialisationSectionName.PStringData = 0;      if (!f(t)) fail = 1;   // no block = ""
    if (fail) { std::printf("006a7c60_TEST FAIL\n"); return 1; }
    std::printf("006a7c60_TEST PASS\n");
    return 0;
}