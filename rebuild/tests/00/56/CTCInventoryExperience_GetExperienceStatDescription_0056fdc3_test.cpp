// Behaviour test for CTCInventoryExperience::GetExperienceStatDescription @ 0x0056FDC3 (model copy with stub lookups).
#include <cstdio>
class CWideString { public:
    const char* PStringData;
    CWideString() : PStringData(0) {}
    CWideString(const CWideString& o) : PStringData(o.PStringData) {}
    ~CWideString() {}
};
static CWideString g_EmptyWideString;
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
class CAbilityTrackLevelDef { public: unsigned char _pad00[0x44]; long Description; };
class CAbilityTrackDef { public:
    unsigned char _pad00[0x40];
    CAbilityTrackLevelDef* Abilities_first; CAbilityTrackLevelDef* Abilities_last; CAbilityTrackLevelDef* Abilities_end;
    long MaxedOutDescription;
};
class CHeroStatsDef { public:
    unsigned char _pad00[0xfc];
    CAbilityTrackDef* StatLevels_first; CAbilityTrackDef* StatLevels_last; CAbilityTrackDef* StatLevels_end;
};
struct CDefinitionHolder { unsigned char _pad_0x00[0xDC]; CHeroStatsDef* PHeroStatsDef; };
class CTCHeroExperience { public:
    long maxLevel[4];
    long calls;
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) { ++calls; return maxLevel[(long)stat]; }
};
struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long* key) { Entry* e = first; while (e != last && e->type < *key) ++e; return e; }
};
struct CPresenceBits { bool test(long kind) { return kind == 0x68 || kind == 0x10; } };   // the thing carries kinds 0x10 and 0x68
class CThing { public:
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
static const char* g_texts[] = { "", "text1", "text2", "text3", "text4", "text5", "text6", "text7", "text8", "text9" };
struct CTCInventoryBase {
    void* __vftable; CThing* ParentThing;
    CDefinitionHolder* holder; long textLookups;
    CDefinitionHolder* GetDefinitions() const { return holder; }
    CWideString GetInventoryText(long text_id) { ++textLookups; CWideString s; s.PStringData = g_texts[text_id]; return s; }
};
class CTCInventoryExperience : public CTCInventoryBase { public:
    CWideString GetExperienceStatDescription(long stat, long level);
};
CWideString CTCInventoryExperience::GetExperienceStatDescription(long stat, long level)
{
    CTCHeroExperience* experience;
    ParentThing->GetInterface(&experience, 0x68);
    const CHeroStatsDef* statsDef = GetDefinitions()->PHeroStatsDef;
    long textId;
    if (level == experience->GetTrainableStatMaxLevel((EHeroTrainableStatType)stat) + 1)
        textId = statsDef->StatLevels_first[stat].MaxedOutDescription;
    else
        textId = statsDef->StatLevels_first[stat].Abilities_first[level].Description;
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}
int main()
{
    CAbilityTrackLevelDef levels[2][3];
    CAbilityTrackDef tracks[2];
    for (int t = 0; t < 2; ++t) {
        for (int l = 0; l < 3; ++l) levels[t][l].Description = t == 0 ? (l == 1 ? 0 : 1 + l) : 5 + l;
        tracks[t].Abilities_first = levels[t]; tracks[t].Abilities_last = levels[t] + 3; tracks[t].Abilities_end = levels[t] + 3;
        tracks[t].MaxedOutDescription = t == 0 ? 4 : 9;
    }
    CHeroStatsDef statsDef; statsDef.StatLevels_first = tracks; statsDef.StatLevels_last = tracks + 2; statsDef.StatLevels_end = tracks + 2;
    CDefinitionHolder holder; holder.PHeroStatsDef = &statsDef;
    CTCHeroExperience experience; experience.maxLevel[0] = 2; experience.maxLevel[1] = 1; experience.calls = 0;
    CInterfaceMap::Entry entries[2] = { { 0x10, 0 }, { 0x68, &experience } };
    CThing thing;
    thing.interfaces.first = entries; thing.interfaces.last = entries + 2;
    CTCInventoryExperience inv; inv.ParentThing = &thing; inv.holder = &holder; inv.textLookups = 0;
    g_EmptyWideString.PStringData = "";
    bool ok = true;
    CWideString a = inv.GetExperienceStatDescription(0, 0);      // level 0 of stat 0 -> text 1
    if (a.PStringData != g_texts[1] || experience.calls != 1 || inv.textLookups != 1) ok = false;
    CWideString b = inv.GetExperienceStatDescription(0, 3);      // max level 2 + 1 -> maxed-out text 4
    if (b.PStringData != g_texts[4] || inv.textLookups != 2) ok = false;
    CWideString c = inv.GetExperienceStatDescription(0, 1);      // level 1 of stat 0 has no text -> empty, no lookup
    if (c.PStringData != g_EmptyWideString.PStringData || inv.textLookups != 2) ok = false;
    CWideString d = inv.GetExperienceStatDescription(1, 2);      // stat 1 max 1 + 1 -> maxed-out text 9
    if (d.PStringData != g_texts[9] || inv.textLookups != 3 || experience.calls != 4) ok = false;
    CWideString e = inv.GetExperienceStatDescription(1, 1);      // level 1 of stat 1 -> text 6
    if (e.PStringData != g_texts[6]) ok = false;
    if (!ok) { std::printf("0056fdc3_TEST FAIL\n"); return 1; }
    std::printf("0056fdc3_TEST PASS\n");
    return 0;
}