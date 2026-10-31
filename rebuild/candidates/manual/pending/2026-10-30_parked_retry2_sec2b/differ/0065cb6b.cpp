// CTCVillage::ReportCrime(long deedHandle) -- record a crime against the village: tell the hero's deed log, add its fine to the hero's crime stats (and the online scoreboard), for a murder mark the village hostile, drop the assault/theft crimes against the same victim and count the murder, for an assault/theft fold it into a known murder; keep the handle in the village crime set
// Grade: DIFFER(747v747) 14 difflines: the murder arm takes its own CTCHeroStats* local (retail re-fetches it), which frees the deed log slot (ebp-0xc) and register roles; residue one frame word: the scoreboard key 0x10d gets its own slot (ebp-18h) where retail shares ebp-14h with the first stats key (retry wave 2 sec2b)
// Was: DIFFER(748v747) -- frame 0x1c vs retail 0x18: retail shares one slot for the two key temporaries (ebp-0x14) and keeps the deed log in ebp-0xc; ours gives each key temp its own slot, so ebp-8/ebp-0xc swap and the deed log pointer is re-read from ebp+8 once
// Leaf: ?ReportCrime@CTCVillage@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065CB6B, 747 bytes, `ret 4`, ebp frame (size TU: optimize("y",off) + ("s",on)).
//   Ego_r.pdb ?ReportCrime@CTCVillage@@QAEXJ@Z.
// Callees: CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0, std::bitset<274>::test 0x00410DE0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC<T>),
//   CTCHeroOpinionDeedLog::SetVillageForCrimes 0x006C55E0 / SetCrimeDeedReportedToVillage 0x006C5A50 / GetDeedFromHandle
//   0x006C58F0 / PeekPDeedEffectsDef 0x006C5630 / FindCriminalDeed 0x006C5800 (tests COpinionDeed::IsCrime +0x23; retail
//   order OnInitialActivate 0x006C5680, FindWitnessDeed 0x006C5710, FindDeed 0x006C5780, FindCriminalDeed 0x006C5800),
//   CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0, CTCVillage::RemoveCrime 0x0065C146,
//   std::set<long>::find 0x00614A10 / insert 0x00514230.
// TC kinds (GetInterfaceType, RTTI slot 21): 0x51 CTCHeroOpinionDeedLog, 0x04 CTCHeroStats, 0x10D CTCHeroOnlineScoreboard.
// Layout: retail CTCBase ParentThing +0x04; CTCVillage Crimes set<long> +0x64, IsEnemyBecauseOfCrime +0x7C; retail
//   CThing bitset +0x20, CVectorMap +0x44; CPlayer PlayerCharacter +0x34; COpinionDeed DeedType +0x00, PTarget +0x18;
//   retail COpinionDeedEffectsDef CrimeFineAmount +0x74 (Ego_r +0x88); retail CTCHeroStats NumberOfCrimes +0xF4,
//   NumberOfCrimesOfMurder +0xF8, TotalFine +0xFC (Ego_r +0xFC..+0x104); CMainGameComponent World +0x24.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class CThing;
class CBaseObject;
namespace std {
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;               // 0x00410DE0
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Tp> struct _Rb_tree_iterator { void* _M_node; };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    void* _M_header;
    unsigned int _M_node_count;
    iterator find(const _Key& __x);                     // 0x00614A10
    iterator end() { iterator __it; __it._M_node = _M_header; return __it; }
    pair<iterator, bool> insert(const _Key& __x);       // 0x00514230
};
}
enum ETCInterfaceType {
    TC_HERO_STATS = 0x04,
    TC_HERO_OPINION_DEED_LOG = 0x51,
    TC_HERO_ONLINE_SCOREBOARD = 0x10d
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    iterator LowerBound(const K& key);                  // 0x0040F020
    iterator end() { return _M_finish; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;  // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC((ETCInterfaceType)T::InterfaceType))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        *out = (T*)(p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first ? components.end() : p)->second;
        return true;
    }
};
class CThingPlayerCreature : public CThing {
};
class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    void* PInfo;
};
class CPlayer {
public:
    unsigned char _pad00[0x34];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CThingPlayerCreature* GetPControlledCreature() const;        // 0x00487DC0
    CThingPlayerCreature* GetPPlayerCharacter() const { return (CThingPlayerCreature*)PlayerCharacter.GetPItem(); }
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
class CWorld {
public:
    const CPlayer* PeekMyPlayer() const;                // 0x0049D9D0
};
class CMainGameComponent {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // slot 1
    mutable unsigned long IVRefCount;                   // +0x04
    __forceinline void Release() const
    {
        if (--IVRefCount == 0)
            Delete();
    }
};
class CResource : public CIVCountedPointeeBase {
};
class CDefPointeeBase : public CResource {
};
template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline ~CDefPointer() { Release(); }
    __forceinline void Release()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class COpinionDeedEffectsDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x74 - 0x08];
    long          CrimeFineAmount;                      // +0x74 (Ego_r +0x88)
};
enum EOpinionDeedType {
    DEED_TYPE_ASSAULT_SEVEN = 7,
    DEED_TYPE_EIGHT = 8,
    DEED_TYPE_MURDER_NINE = 9
};
class COpinionDeed {
public:
    EOpinionDeedType DeedType;                          // +0x00
    unsigned char    _pad04[0x18 - 0x04];
    CIntelligentPointer<const CThing> PTarget;          // +0x18
};
class CTCHeroOpinionDeedLog {
public:
    enum { InterfaceType = TC_HERO_OPINION_DEED_LOG };
    void SetVillageForCrimes(CThing& village);                                   // 0x006C55E0
    void SetCrimeDeedReportedToVillage(long handle);                             // 0x006C5A50
    const COpinionDeed* GetDeedFromHandle(long handle) const;                    // 0x006C58F0
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
    long FindCriminalDeed(EOpinionDeedType type, const CThing* target);          // 0x006C5800
};
class CTCHeroStats {
public:
    enum { InterfaceType = TC_HERO_STATS };
    unsigned char _pad00[0xf4];
    long NumberOfCrimes;                                // +0xF4
    long NumberOfCrimesOfMurder;                        // +0xF8
    long TotalFine;                                     // +0xFC
};
class CTCHeroOnlineScoreboard {
public:
    enum { InterfaceType = TC_HERO_ONLINE_SCOREBOARD };
    enum EOnlineScoreboardActivity { };
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool lowerIsBetter);   // 0x0055F3B0
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
protected:
    CPlayerManager& GetPlayerManager();                 // 0x00686D40
};
class CTCVillage : public CTCBase {
public:
    unsigned char  _pad08[0x64 - 0x08];
    std::set<long> Crimes;                              // +0x64
    unsigned char  _pad6c[0x7c - 0x6c];
    bool           IsEnemyBecauseOfCrime;               // +0x7C
    void RemoveCrime(long deedHandle);                  // 0x0065C146
    void ReportCrime(long deedHandle);
};

void CTCVillage::ReportCrime(long deedHandle)
{
    CThingPlayerCreature* hero = g_pMainGameComponent->World->PeekMyPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetTC(&log);
        log->SetVillageForCrimes(*ParentThing);
        log->SetCrimeDeedReportedToVillage(deedHandle);
        const COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed) {
            CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef(deed->DeedType);
            long fine = effects->CrimeFineAmount;
            CTCHeroStats* stats;
            if (fine != -1 && hero->GetTC(&stats)) {
                ++stats->NumberOfCrimes;
                stats->TotalFine += fine;
                CTCHeroOnlineScoreboard* scoreboard;
                GetPlayerManager().GetMainPlayer().GetPPlayerCharacter()->GetTC(&scoreboard);
                scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x23, (float)stats->TotalFine, false);
                scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x22, (float)stats->NumberOfCrimes, false);
            }
            if (deed->DeedType == DEED_TYPE_MURDER_NINE) {
                IsEnemyBecauseOfCrime = true;
                long related = log->FindCriminalDeed(DEED_TYPE_ASSAULT_SEVEN, (const CThing*)deed->PTarget.GetPItem());
                if (related != -1)
                    RemoveCrime(related);
                related = log->FindCriminalDeed(DEED_TYPE_EIGHT, (const CThing*)deed->PTarget.GetPItem());
                if (related != -1)
                    RemoveCrime(related);
                CTCHeroStats* murderStats;
                hero->GetTC(&murderStats);
                ++murderStats->NumberOfCrimesOfMurder;
                CTCHeroOnlineScoreboard* scoreboard;
                hero->GetTC(&scoreboard);
                scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0, (float)murderStats->NumberOfCrimesOfMurder, false);
            } else if (deed->DeedType == DEED_TYPE_ASSAULT_SEVEN || deed->DeedType == DEED_TYPE_EIGHT) {
                long murder = log->FindCriminalDeed(DEED_TYPE_MURDER_NINE, (const CThing*)deed->PTarget.GetPItem());
                if (murder != -1) {
                    if (Crimes.find(murder)._M_node == Crimes.end()._M_node)
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
