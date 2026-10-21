// CTCInventoryStats::BuildHeroClass() -- build the hero-class text: TEXT_GUI_STATS_HEROCLASS_ + PHYSICAL_/SKILL_/MAGIC_ (largest Strength/Skill/Will experience) + the highest stat level name (or NONE), looked up in the text bank
// Grade: DIFFER(483v499) -- the `first` bool and the best/bestIndex locals do not share retail's slots (16 bytes more stack traffic in retail); /Os TU
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
struct CCharString {
    void* m_pStringData;
    CCharString();                                                // 0x0099E4B0
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
    CCharString& operator=(const char* text);                     // 0x0099EFE0
    CCharString& operator+=(const CCharString& other);            // 0x0099F0A0
};
struct CWideString {
    void* m_pStringData;
    CWideString();                                                // 0x0099AED0
    ~CWideString();                                               // 0x0099B510
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);       // 0x009C95E0
};
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};
extern CEngineGlobals* g_pEngineGlobals;                          // 0x013B86A0

struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    long& operator[](unsigned int n) { return *(_M_start + n); } };
struct CTCHeroStats {                                 // interface type 4
    unsigned char _pad_0x00[0x18];
    CLongVector StatLevels;                           // +0x18
    long GetHeroStatExperience(long category);        // 0x0057A8AF
};

struct CTCInventoryStats {
    void*   __vftable;
    CThing* ParentThing;                              // +0x04
    CWideString BuildHeroClass();
};

CWideString CTCInventoryStats::BuildHeroClass()
{
    CWideString text;
    CTCHeroStats* stats = ParentThing->GetTC<CTCHeroStats>(4);
    CCharString symbol("TEXT_GUI_STATS_HEROCLASS_");
    long strength = stats->GetHeroStatExperience(0);
    long skill = stats->GetHeroStatExperience(1);
    long will = stats->GetHeroStatExperience(2);
    CCharString category;
    if (strength >= skill && strength >= will)
        category = "PHYSICAL_";
    else if (skill >= strength && skill >= will)
        category = "SKILL_";
    else if (will >= strength && will >= skill)
        category = "MAGIC_";
    symbol += category;
    bool first = true;
    long best = 0;
    long bestIndex = 0;
    unsigned int count = stats->StatLevels.size();
    for (unsigned int i = 0; i != count; ++i) {
        if (first || stats->StatLevels[i] >= best) {
            best = stats->StatLevels[i];
            bestIndex = i;
            first = false;
        }
    }
    CCharString stat;
    if (best == 0)
        stat = "NONE";
    else {
        if (bestIndex == 0)
            stat = "STRENGTH";
        if (bestIndex == 2)
            stat = "TOUGHNESS";
        if (bestIndex == 1)
            stat = "HEALTH";
        if (bestIndex == 3)
            stat = "SPEED";
        if (bestIndex == 4)
            stat = "ACCURACY";
        if (bestIndex == 5)
            stat = "GUILE";
        if (bestIndex == 9)
            stat = "MAGICPOWER";
        if (bestIndex == 10)
            stat = "SPELLS";
    }
    symbol += stat;
    return g_pEngineGlobals->m_pTextBank->GetTextBySymbol(symbol);
}
