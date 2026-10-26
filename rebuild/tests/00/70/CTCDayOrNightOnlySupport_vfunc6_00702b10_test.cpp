// Behaviour test for CTCDayOrNightOnlySupport::OnInitialActivate (0x00702B10): the quest manager's suffix of the section name
// selects 1 ("DayOnly"), 2 ("NightOnly") or 0; equality by shared block or by equal length + Compare; the suffix is destroyed.
#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
long Compare_00411570(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
static int g_dtors = 0;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString() { ++g_dtors; }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
static CCharStringData g_day = { "DayOnly", 7 }, g_night = { "NightOnly", 9 };
CCharString g_DayOnlySuffix = { &g_day };
CCharString g_NightOnlySuffix = { &g_night };
static CCharStringData* g_result = 0; static const CCharString* g_asked = 0;
class CQuestManager { public:
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& sectionName) { g_asked = &sectionName; CCharString r = { g_result }; return r; }
};
static CQuestManager g_qm;
CQuestManager* g_pQuestManager = &g_qm;
class CThing { public: CCharString SerialisationSectionName; };
class CTCDayOrNightOnlySupport { public:
    virtual void v00() { }
    CThing* ParentThing;
    long DayNightOrBoth;
    void vfunc_6();
};

void CTCDayOrNightOnlySupport::vfunc_6()
{
    CCharString suffix = g_pQuestManager->GetDayOrNightOnlyStringFromFullSectionName(ParentThing->SerialisationSectionName);
    if (suffix == g_DayOnlySuffix) {
        DayNightOrBoth = 1;
        return;
    }
    if (suffix == g_NightOnlySuffix) {
        DayNightOrBoth = 2;
        return;
    }
    DayNightOrBoth = 0;
}

int main()
{
    int bad = 0;
    CThing thing; thing.SerialisationSectionName.PStringData = 0;
    CTCDayOrNightOnlySupport s; s.ParentThing = &thing; s.DayNightOrBoth = -1;
    static CCharStringData night2 = { "NightOnly", 9 }, other = { "Nightonly", 9 }, shortDay = { "Day", 3 };
    g_result = &g_day; s.vfunc_6();                  // the same block
    if (s.DayNightOrBoth != 1 || g_asked != &thing.SerialisationSectionName) bad |= 1;
    g_result = &night2; s.vfunc_6();                 // equal text in another block
    if (s.DayNightOrBoth != 2) bad |= 2;
    g_result = &other; s.vfunc_6();                  // same length, different text
    if (s.DayNightOrBoth != 0) bad |= 4;
    g_result = &shortDay; s.DayNightOrBoth = 5; s.vfunc_6();
    if (s.DayNightOrBoth != 0) bad |= 8;
    g_result = 0; s.DayNightOrBoth = 5; s.vfunc_6(); // empty string
    if (s.DayNightOrBoth != 0) bad |= 16;
    if (bad) { std::printf("00702b10_TEST FAIL\n"); return 1; }
    std::printf("00702b10_TEST PASS\n");
    return 0;
}