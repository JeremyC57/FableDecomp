// Grade: DIFFER(750v747) -- deed log homed in memory after the fine block (retail keeps it in esi with a reload) and the stats key 4 CSE'd into esi; frame 0x1c v 0x18
// CTCVillage::ReportCrime(long deedHandle) -- record a crime against the village: tell the hero's deed log, add its fine to the hero's crime stats (and online scoreboard), for a murder mark the village hostile and fold the related crimes, for an assault/theft fold it into a known murder, and keep the handle in the village crime set
// Leaf: ?ReportCrime@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065CB6B, 747 bytes, `ret 4`, ebp frame (size TU, `optimize("s",on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline void GetInterfaceB(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CPlayer {
    unsigned char _pad[0x34];
    CBaseIntelligentPointer Hero;               // +0x34
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct COpinionDeedEffectsDef : CDefVirtual {
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x74 - 0x08];
    long          CrimeFineAmount;              // +0x74 (descriptive)
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
struct COpinionDeed {
    long          DeedType;                     // +0x00
    unsigned char _pad04[0x18 - 0x04];
    CBaseIntelligentPointer Victim;             // +0x18
};
struct CTCHeroOpinionDeedLog {
    void SetVillage(CThing* village);           // 0x006C55E0
    void SetCrimeDeedReportedToVillage(long handle);   // 0x006C5A50
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
    long FindDeedOfTypeAgainst(long deedType, CThing* victim);   // 0x006C5800
};
struct CTCHeroStats {
    unsigned char _pad00[0xf4];
    long NumberOfCrimes;                        // +0xf4
    long NumberOfCrimesOfMurder;                // +0xf8
    long TotalFine;                             // +0xfc
};
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float score, bool lowerIsBetter);   // 0x0055F3B0
};
struct CCrimeSet {                              // set<long>
    void* _M_header;
    unsigned char _pad[4];
    void** find(const long& key);               // 0x00614A10
    struct pair_ib { void* it; bool second; };
    pair_ib insert(const long& key);            // 0x00514230
    void** end() { return (void**)_M_header; }
};

struct CTCVillage {
    void* _vfptr;
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x64 - 0x08];
    CCrimeSet Crimes;                           // +0x64
    unsigned char _pad6c[0x7c - 0x6c];
    bool IsEnemyBecauseOfCrime;                 // +0x7c
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    void RemoveCrime(long deedHandle);          // 0x0065C146
    void ReportCrime(long deedHandle);
};

void CTCVillage::ReportCrime(long deedHandle)
{
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterfaceB(&log, 0x51);
        log->SetVillage(ParentThing);
        log->SetCrimeDeedReportedToVillage(deedHandle);
        COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed) {
            CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed->DeedType);
            long fine = effects->CrimeFineAmount;
            CTCHeroStats* stats;
            if (fine != -1 && hero->GetInterface(&stats, 4)) {
                ++stats->NumberOfCrimes;
                stats->TotalFine += fine;
                CTCHeroOnlineScoreboard* scoreboard;
                GetPlayerManager()->GetMainPlayer()->Hero.GetPItem()->GetInterface(&scoreboard, 0x10d);
                scoreboard->UpdateScoreForActivity(0x23, (float)stats->TotalFine, false);
                scoreboard->UpdateScoreForActivity(0x22, (float)stats->NumberOfCrimes, false);
            }
            if (deed->DeedType == 9) {
                IsEnemyBecauseOfCrime = true;
                long related = log->FindDeedOfTypeAgainst(7, deed->Victim.GetPItem());
                if (related != -1)
                    RemoveCrime(related);
                related = log->FindDeedOfTypeAgainst(8, deed->Victim.GetPItem());
                if (related != -1)
                    RemoveCrime(related);
                hero->GetInterface(&stats, 4);
                ++stats->NumberOfCrimesOfMurder;
                CTCHeroOnlineScoreboard* scoreboard;
                hero->GetInterface(&scoreboard, 0x10d);
                scoreboard->UpdateScoreForActivity(0, (float)stats->NumberOfCrimesOfMurder, false);
            } else if (deed->DeedType == 7 || deed->DeedType == 8) {
                long murder = log->FindDeedOfTypeAgainst(9, deed->Victim.GetPItem());
                if (murder != -1) {
                    if (Crimes.find(murder) == Crimes.end())
                        ReportCrime(murder);
                    RemoveCrime(deedHandle);
                    deedHandle = -1;
                }
            }
        }
    }
    if (deedHandle != -1)
        Crimes.insert(deedHandle);
}
