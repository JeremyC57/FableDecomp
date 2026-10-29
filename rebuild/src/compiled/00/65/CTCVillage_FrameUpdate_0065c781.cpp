// CTCVillage::FrameUpdate() -- per-frame village upkeep: prune members, refresh tasks and crowds, run the def's task updaters, crimes and enemy status; every 30 frames switch the village lights with the day and recount the working villagers; play the curfew / cook-time change announcement near the hero; then update the curfew
// Leaf: ?FrameUpdate@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065C781, 1002 bytes, plain `ret`, ebp frame (size TU, `optimize("s",on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    bool operator!=(const char* text) const;           // 0x0099E960
    const char* GetString() const;                     // 0x00404310
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
    long GetVillagerState();                    // 0x00662010 (descriptive)
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE_004cc340(const CBaseIntelligentPointer& thing);   // 0x004CC340
bool __fastcall IS_THING_ALIVE_Villager(const CBaseIntelligentPointer& thing);   // 0x004AB8F0
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CASoundBank { public:
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
class CSoundInit { public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit();                               // 0x0041C930
};
class CThingSoundEmitter : public CThing {
public:
    static CThingSoundEmitter* __fastcall Create(const C3DVector& position, const CThing* parent, bool unattached);   // 0x008369F0
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);       // 0x00836810
};
bool __fastcall IS_THING_ALIVE(CThingSoundEmitter* const& thing);   // 0x0065D55F
class CSymbolMap { public:
    CASoundBank* FindFirstSymbolWithVal(const CCharString* value);   // 0x009917E0
};
extern CSymbolMap* g_pSoundBankMap;             // 0x013B8394
struct CGroupEntryRef {                         // CDefPointer-like counted pointer to the text group entry
    struct CRefCounted {
        long m_count;
        void DecRef() { --m_count; }
        long GetRefCount() const { return m_count; }
    };
    struct CEntryBase {
        virtual void v00();
        virtual void Destroy();
    };
    struct CEntry : CEntryBase, CRefCounted {
        __forceinline void ReleaseTemporary()
        {
            DecRef();
            if (GetRefCount() == 0)
                Destroy();
        }
        unsigned char _pad08[0x1c - 0x08];
        CCharString BankName;                   // +0x1c
        unsigned char _pad20[4];
        CCharString SoundName;                  // +0x24
    };
    CEntry* Object;
    __forceinline ~CGroupEntryRef()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CTextBank {
    long LookupValue(const char* name);         // 0x009CC410
    bool EntryExists(long id);                  // 0x009CC190
    CGroupEntryRef GetPGroupEntry(long id, bool create);   // 0x009C9070
};
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CTextBank* PTextBank;                       // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern bool g_RepopulateVillages;               // 0x013B86E9
class CGameTimeManager { public:
    bool IsDay();                               // 0x006BB2C0
};
class CWorld { public:
    unsigned char _pad00[0x1c];
    CGameTimeManager* PTimeManager;             // +0x1c
    static long __fastcall GetFrame();                // 0x0049D870
};
class CWorldMap { public:
    void PopulateRegionWithVillagers(CThing* village);   // 0x00502E90
};
class CTCLight { public:
    unsigned char _pad00[0x1c];
    bool OnDuringDay;                           // +0x1c (descriptive)
    void SetActive(bool active);                // 0x006412B0
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
template <class T> struct SNode {
    SNode* _M_next;
    SNode* _M_prev;
    T      _M_data;
};
class CVillageDef { public:
    unsigned char _pad00[0x28];
    long* TaskTypesStart;                       // +0x28
    long* TaskTypesFinish;                      // +0x2c
    unsigned char _pad30[0xe6 - 0x30];
    bool HasCurfewAnnouncements;                // +0xe6
    unsigned char _pad_e7[0xe8 - 0xe7];
    CCharString CurfewEndText;                  // +0xe8
    CCharString CurfewStartText;                // +0xec
    CCharString CookEndText;                    // +0xf0
    CCharString CookStartText;                  // +0xf4
};
class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    CWorldMap& GetWorldMap();   // 0x00686D30
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCVillage : public CTCBase { public:
    void* _vfptr;
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x2c - 0x08];
    SNode<CBaseIntelligentPointer>* VillagePeople;   // +0x2c
    SNode<CBaseIntelligentPointer>* Lights;     // +0x30
    unsigned char _pad34[0x88 - 0x34];
    long NumWorkingVillagers;                   // +0x88
    unsigned char _pad8c[0x94 - 0x8c];
    CVillageDef* PDef;                          // +0x94
    unsigned char _pad98[0xb0 - 0x98];
    bool WasCookTime;                           // +0xb0
    bool WasCurfewTime;                         // +0xb1
    unsigned char _padb2;
    bool AnnouncementsDisabled;                 // +0xb3
    void ClearOutNullMembers();                 // 0x0065890B
    void ReleaseInvalidTasks();                 // 0x0065BE32
    void UpdateCrowds();                        // 0x006594BB
    void UpdateTorchTasks();                    // 0x00659625
    void UpdateSecuritySweepTasks();            // 0x0065C08A
    void UpdateHornTasks();                     // 0x0065BE6C
    void SetEnemyToHeroStatus();                // 0x0065A852
    void RemoveTimedoutCrimes();                // 0x0065A7A9
    void UpdateMirroredCrimeData();             // 0x0065C5BB
    bool IsCurfewTime_00658db3();                        // 0x00658DB3
    bool IsCookTime_00658b20();                          // 0x00658B20
    void UpdateCurfew();                        // 0x0065AD37
    void FrameUpdate();
};

void CTCVillage::FrameUpdate()
{
    ClearOutNullMembers();
    ReleaseInvalidTasks();
    UpdateCrowds();
    if (g_RepopulateVillages) {
        GetWorldMap().PopulateRegionWithVillagers(ParentThing);
        g_RepopulateVillages = false;
    }
    bool torchesUpdated = false;
    for (long* type = PDef->TaskTypesStart; type != PDef->TaskTypesFinish; ++type) {
        switch (*type) {
        case 0:
            UpdateTorchTasks();
            torchesUpdated = true;
            break;
        case 3:
            UpdateSecuritySweepTasks();
            break;
        case 4:
            UpdateHornTasks();
            break;
        }
    }
    SetEnemyToHeroStatus();
    RemoveTimedoutCrimes();
    if (CWorld::GetFrame() % 30 == 0) {
        if (!torchesUpdated) {
            for (SNode<CBaseIntelligentPointer>* node = Lights->_M_next; node != Lights; node = node->_M_next) {
                if (IS_THING_ALIVE_004cc340(node->_M_data)) {
                    CThing* thing = node->_M_data.GetPItem();
                    if (thing->HasTC(0x27)) {
                        CTCLight* light = (CTCLight*)thing->GetTC(0x27);
                        bool onDuringDay = light->OnDuringDay;
                        if (onDuringDay == PeekWorld().PTimeManager->IsDay())
                            light->SetActive(!PeekWorld().PTimeManager->IsDay());
                    }
                }
            }
        }
        NumWorkingVillagers = 0;
        SNode<CBaseIntelligentPointer>* end = VillagePeople;
        for (SNode<CBaseIntelligentPointer>* node = end->_M_next; node != end; node = node->_M_next) {
            if (IS_THING_ALIVE_Villager(node->_M_data) && node->_M_data.GetPItem()->GetVillagerState() == 3)
                ++NumWorkingVillagers;
        }
        UpdateMirroredCrimeData();
    }
    if (PDef->HasCurfewAnnouncements && !AnnouncementsDisabled) {
        CCharString text(0);
        bool was = WasCurfewTime;
        if (was != IsCurfewTime_00658db3())
            text = was ? PDef->CurfewStartText : PDef->CurfewEndText;
        was = WasCookTime;
        if (was != IsCookTime_00658b20())
            text = was ? PDef->CookStartText : PDef->CookEndText;
        if (text != "") {
            CTextBank* bank = g_pMainGameComponent->PTextBank;
            long id = bank->LookupValue(text.GetString());
            bool exists = bank->EntryExists(id);
            CGroupEntryRef::CEntry* entry = bank->GetPGroupEntry(id, false).Object;
            if (entry || !exists) {
                WasCurfewTime = IsCurfewTime_00658db3();
                WasCookTime = IsCookTime_00658b20();
                if (entry) {
                    CASoundBank* soundBank = g_pSoundBankMap->FindFirstSymbolWithVal(&entry->BankName);
                    if (soundBank) {
                        long sample = soundBank->GetSoundIndexFromSymbol("SND_" + entry->SoundName);
                        if (sample > 0) {
                            C3DVector pos = ParentThing->GetPos();
                            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer()->GetPControlledCreature();
                            if (IS_THING_ALIVE(hero)) {
                                C3DVector offset = hero->PhysicsTC->GetRHSet().Forward * 20.0f;
                                pos = hero->GetPos() + offset;
                            }
                            {
                                CThingSoundEmitter* emitter = CThingSoundEmitter::Create(pos, 0, true);
                                if (IS_THING_ALIVE(emitter)) {
                                    CSoundInit init;
                                    init.GainFaderBits = 0x8040;
                                    emitter->PlaySample(*soundBank, sample, false, init, false);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    UpdateCurfew();
}