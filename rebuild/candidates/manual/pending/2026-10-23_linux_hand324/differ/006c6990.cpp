// CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType, const C3DVector&, const CThing*, CThingAICreature*, EOpinionPostDeedType, bool, bool, const CFaction*, bool, long) -- records a new opinion deed: the hero's current region def gates crimes (type 1 needs both region flags, otherwise crimes there are not shown/flagged), the villager-faction and guard/village filters clear the show flag, then the deed is created under the next handle, optionally queued for witnesses, and dropped again when nobody was informed; finally the deed's tutorial is given when tutorials are on
// Leaf: PostDeed
// Retail: Fable.exe @ 0x006C6990, 1168 bytes, `ret 0x28`. Ego_r.pdb `?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@_N5PBVCFaction@@5J@Z`.
// Callees: CTCBase::GetWorldMap 0x00686D30, CThing::GetPosition 0x004C73D0, the world map's slot 16, GetRegionNumberMapIsIn
// 0x004FC190, GetRegionWithIndex 0x004FB490 (as the landed CTCVillage::GetNormalisedTimeLeftOnFines 0x0065A28F),
// CThingDef::GetSubDef 0x006C7720, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
// GetDefGlobalIndexFromName 0x009AD410, CBaseIntelligentPointer::GetPItem 0x00A01B50 / ~ 0x00A01C10, LowerBound 0x0040F020,
// operator new 0x00BFEA1A / delete 0x00BFE9BC, COpinionDeed ctor 0x006C54C0, map insert 0x006C8400 / erase 0x006C9220,
// CWitnessesSearchingDeed ctor 0x006C5410, list push_back 0x006C91E0, InformRelevantThingsOfDeed 0x006C6040,
// GetDeedEffectsDef 0x006C5630, CTCHero::GiveTutorialForCategory 0x006C9FA0; g_tutorialsEnabled 0x01375740,
// g_pPlayerGui 0x013B8790.
// Layout: retail OpinionDeedMap +0x0c (12-byte map), OpinionDeedsSearchingForWitnesses +0x18 (4-byte list), NextHandle
// +0x1c, VillageForCrimes +0x20, GuardsIgnoreCrimes +0x2d (PDB +0x0c/+0x1c/+0x28/+0x2c/+0x39 with 16-byte containers).
// Grade: DIFFER(1183v1168) -- first draft: frame 0x40 v 0x3c, region-def addref value CSE (dec eax v dec [eax+4]), register assignment in the faction block (bl flag v byte slot)
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CFaction;
struct CThingAICreature;

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_count;                                    // +0x04
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object)
    {
        if (Object)
            ++Object->m_count;
    }
    __forceinline ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CRegionDef : CDefBase {
    unsigned char _pad08[0x94];
    bool CrimesEnabled;                              // +0x9c (descriptive)
    bool HasFines;                                   // +0x9d
    bool ShowsCrimes;                                // +0x9e (descriptive)
};
struct CFactionDef : CDefBase {
    unsigned char _pad08[0x20];
    long GlobalIndex;                                // +0x28
};
struct CThingDef : CDefBase {
    bool GetSubDef(CDefPointer<CFactionDef>* out) const;   // 0x006C7720
};
struct COpinionDeedEffectsDef : CDefBase {
    unsigned char _pad08[0x70];
    long TutorialCategory;                           // +0x78 (descriptive)
};
struct CEffectsDefPointer {
    COpinionDeedEffectsDef* Object;
    __forceinline ~CEffectsDefPointer()
    {
        if (Object) {
            Object->DecRef();
            if (Object->GetRefCount() == 0)
                Object->Destroy();
            Object = 0;
        }
    }
    COpinionDeedEffectsDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_present[2];                      // +0x20
    unsigned long m_flags;                           // +0x28
    unsigned char _pad2c[0x18];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x24];
    CThingDef*    Def;                               // +0x70
    unsigned char _pad74[0x1d];
    unsigned char m_limboFlags;                      // +0x91
    bool HasTC(long type) const { return (m_present[type >> 5] & (1u << (type & 31))) != 0; }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
struct CTCHero {
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
struct CTCVillageTC {
    unsigned char _pad[0xb3];
    bool GuardsIgnore;                               // +0xb3 (descriptive)
};
struct CBaseIntelligentPointer {
    void* m_p;
    void* m_q;
    CThing* GetPItem() const;                        // 0x00A01B50
    ~CBaseIntelligentPointer();                      // 0x00A01C10
};
struct CPlayerGui {
    unsigned char _pad[0xf6];
    bool TutorialsSuppressed;                        // +0xf6 (descriptive)
};
extern bool g_tutorialsEnabled;                      // 0x01375740
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CRegion {
    unsigned char _pad00[0x24];
    CRegionDef*   PDef;                              // +0x24
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapAtPos(const C3DVector& pos);  // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);           // 0x004FC190
    CRegion* GetRegionWithIndex(long region);        // 0x004FB490
};

struct COpinionDeed {                                // 0x28 bytes
    long          DeedType;                          // +0x00
    long          DeedHandle;                        // +0x04
    C3DVector     Position;                          // +0x08
    long          FrameHappened;                     // +0x14
    CBaseIntelligentPointer PTarget;                 // +0x18
    bool          WasSearchingForWitnesses;          // +0x20
    bool          IsStillHappening;                  // +0x21
    bool          IsCrimeReportedToVillage;          // +0x22
    bool          IsCrime;                           // +0x23
    long          ExtraData;                         // +0x24
    COpinionDeed(long type, long handle, const C3DVector& pos, const CThing* thing, bool b1, bool b2, long param);   // 0x006C54C0
};
struct CWitnessesSearchingDeed {
    long DeedHandle;                                 // +0x00
    long DeedType;                                   // +0x04
    CBaseIntelligentPointer Recipient;               // +0x08
    long PostType;                                   // +0x10
    const CFaction* PFaction;                        // +0x14
    bool ReverseFactionSearch;                       // +0x18
    CWitnessesSearchingDeed(long handle, long type, CThingAICreature* creature, long postType, const CFaction* faction, bool flag);   // 0x006C5410
};
struct SDeedPair { long first; COpinionDeed* second; };
struct SInsertResult { void* it; bool inserted; };
struct CDeedMap {
    void* _M_header[3];
    SInsertResult insert(const SDeedPair& value);    // 0x006C8400
    unsigned int erase(const long& key);             // 0x006C9220
};
struct CWitnessList {
    void* _M_node;
    void push_back(const CWitnessesSearchingDeed& x);   // 0x006C91E0
};

class CTCHeroOpinionDeedLog {
public:
    void*         __vftable;                         // +0x00
    CThing*       Thing;                             // +0x04
    long          m_8;                               // +0x08
    CDeedMap      OpinionDeedMap;                    // +0x0c
    CWitnessList  OpinionDeedsSearchingForWitnesses; // +0x18
    long          NextHandle;                        // +0x1c
    CBaseIntelligentPointer VillageForCrimes;        // +0x20
    long          WeaponOutDeedHandle;               // +0x28
    bool          WeaponOutCrimeEnabled;             // +0x2c
    bool          GuardsIgnoreCrimes;                // +0x2d
    CWorldMap* GetWorldMap();                        // 0x00686D30
    bool InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, long postType, const CFaction* faction, bool flag);   // 0x006C6040
    CEffectsDefPointer GetDeedEffectsDef(long type);   // 0x006C5630
    long PostDeed(long type, const C3DVector& pos, const CThing* thing, CThingAICreature* creature, long postType,
                  bool flagA, bool searchForWitnesses, const CFaction* faction, bool flagB, long param);
};

long CTCHeroOpinionDeedLog::PostDeed(long type, const C3DVector& pos, const CThing* thing, CThingAICreature* creature, long postType,
                                     bool flagA, bool searchForWitnesses, const CFaction* faction, bool flagB, long param)
{
    if (type == 0 || postType == 0)
        return -1;
    bool showEffect = true;
    CRegion* region = GetWorldMap()->GetRegionWithIndex(GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapAtPos(Thing->GetPosition())));
    CDefPointer<CRegionDef> regionDef = region->PDef;
    if (regionDef.Object) {
        if (type == 1) {
            if (!regionDef->CrimesEnabled || !regionDef->HasFines)
                return -1;
        }
        else if (!regionDef->HasFines) {
            showEffect = false;
            flagA = false;
            if (type == 0x35)
                param = -1;
        }
        if (!regionDef->ShowsCrimes)
            showEffect = false;
    }
    else {
        if (type == 1)
            return -1;
        showEffect = false;
        flagA = false;
        if (type == 0x35)
            param = -1;
    }
    if (type == 7 || type == 8 || type == 9) {
        if (thing && (thing->m_flags & 0x8000) == 0) {
            showEffect = false;
            flagA = false;
        }
        CDefPointer<CFactionDef> factionDef;
        if (thing && CDefPointer<CThingDef>(thing->Def)->GetSubDef(&factionDef)
            && factionDef->GlobalIndex != CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FACTION_VILLAGERS"))
            showEffect = false;
    }
    if (GuardsIgnoreCrimes == true) {
        flagA = false;
        showEffect = false;
        if (type == 0x35)
            param = -1;
    }
    if (VillageForCrimes.GetPItem() && (VillageForCrimes.GetPItem()->m_limboFlags & 1) == 0 && VillageForCrimes.GetPItem()->HasTC(0x22)) {
        if (((CTCVillageTC*)VillageForCrimes.GetPItem()->GetTC(0x22))->GuardsIgnore)
            showEffect = false;
    }
    long handle = NextHandle++;
    COpinionDeed* deed = new COpinionDeed(type, handle, pos, thing, flagA, searchForWitnesses, param);
    SDeedPair entry;
    entry.first = handle;
    entry.second = deed;
    OpinionDeedMap.insert(entry);
    if (searchForWitnesses)
        OpinionDeedsSearchingForWitnesses.push_back(CWitnessesSearchingDeed(handle, type, creature, postType, faction, flagB));
    if (!InformRelevantThingsOfDeed(handle, creature, postType, faction, flagB) && !searchForWitnesses) {
        delete deed;
        OpinionDeedMap.erase(handle);
        NextHandle--;
        handle = -1;
    }
    if (showEffect && g_tutorialsEnabled && !g_pPlayerGui->TutorialsSuppressed) {
        CTCHero* hero = 0;
        bool give;
        if (GetDeedEffectsDef(type)->TutorialCategory && Thing->HasTC(0x29)) {
            hero = (CTCHero*)Thing->GetTC(0x29);
            give = true;
        }
        else
            give = false;
        if (give)
            hero->GiveTutorialForCategory(GetDeedEffectsDef(type)->TutorialCategory, false);
    }
    return handle;
}
