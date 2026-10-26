// CQuestManager::ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName) -- debug: create the named quest-card object at the living hero, point its quest card at the given script quest and activate it (loading resources); on success the card becomes active (in limbo, persisted), the guild-seal recall point is reset, its type leaves the available cards and the player GUI refreshes
// Leaf: ActivateTestQuestCard
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
namespace N_00415dd0 { struct CDefString {
    long m_index;
    operator CCharString() const;                        // 0x00415DD0
}; } using N_00415dd0::CDefString;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const long& type);              // 0x0040F020 (the non-const instantiation)
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                              // +0x91 bit 0
    unsigned char _91_1 : 7;
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        CTCMap* map = &Components;
        long key = type;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return it->TC;
    }
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    void SetAsGameStatePersisted(bool persisted);        // 0x004C7880
    CDefString GetDefName();                             // 0x004C7CC0
    const C3DVector& GetPosition() const;                // 0x004C73D0
};

class CQuestCardDef { public:
    virtual void v00();
    virtual void DeleteThis();                           // slot 1
    long RefCount;                                       // +4
};
struct CQuestCardDefPointer {                            // CDefPointer<CQuestCardDef const>
    CQuestCardDef* PDef;
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : PDef(other.PDef) { if (PDef) ++PDef->RefCount; }
    ~CQuestCardDefPointer() { if (PDef && --PDef->RefCount == 0) PDef->DeleteThis(); }
    bool IsNull() const { return PDef == 0; }
};
class CTCQuestCard { public:                                    // TC type 0x6c (retail layout = PDB)
    unsigned char  _pad00[0xc];
    CQuestCardDefPointer PDef;                           // +0x0c
    bool           Finished;                             // +0x10
    bool           Replayable;                           // +0x11
    long           FeatIndex;                            // +0x14
    CCharString    QuestName;                            // +0x18
    CCharString    ObjectiveName;                        // +0x1c
    CCharString    RegionName;                           // +0x20
    CCharString    TeleporterRegionName;                 // +0x24
    CCharString    ScriptQuestName;                      // +0x28
    bool IsActivatable_00702810() const;                 // 0x00702810
    CCharString GetRegionName() const;                   // 0x007025A0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    void* PItem;
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                        // 0x004A96F0 (T = CThing)
};
struct CThingPointerList {
    void* _M_node;
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004BA4E0
};
struct CObjectiveList {
    void* _M_node;
    void clear();                                        // 0x004B8C50
};
class CWorld { public:
    unsigned char _pad00[0x154];
    C3DVector     GuildSealRecallPos;                    // +0x154 (PDB +0x164)
    float         GuildSealRecallAngleXY;                // +0x160 (PDB +0x170)
};
class CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector& position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210
class CPlayer { public:
    unsigned char _pad00[0x28];
    long          PlayerNumber;                          // +0x28
    CThing* GetHeroCreature();                           // 0x00487DC0
    long GetPlayerNumber() const { return PlayerNumber; }
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                       // +0x1c
    CPlayerManager* GetPlayerManager() const { return PlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};
class CPlayerGui { public:
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

class CQuestManager { public:
    unsigned char     _pad00[0x5c];
    CThingPointerList ActiveQuestCards;                  // +0x5c (PDB +0x88)
    unsigned char     _pad60[0x74 - 0x60];
    CObjectiveList    Objectives;                        // +0x74 (PDB +0xc8)
    void*             ScriptInfoManager;                 // +0x78
    CWorld*           World;                             // +0x7c
    bool IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region);   // 0x004B1340
    bool ActivateQuest(const CCharString& name, bool a2, bool loadResources);                 // 0x004B4A10
    void RemoveAvailableQuestCardType(const CCharString& name);                               // 0x004B02F0
    bool ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName);
};

bool CQuestManager::ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName)
{
    CPlayer* player = g_pMainGameComponent->GetPlayerManager()->GetMainPlayer();
    CThing* hero = player->GetHeroCreature();
    if (hero && !hero->Dead) {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(cardDefName);
        if (defIndex > 0) {
            CThing* card = CreateThingObject(defIndex, hero->GetPosition(), player->GetPlayerNumber(), 0, 0, "");
            if (card && card->HasTC(0x6c)) {
                CTCQuestCard* tc = (CTCQuestCard*)card->GetTC(0x6c);
                if (tc) {
                    tc->ScriptQuestName = questName;
                    CQuestCardDef* def = tc->PDef.PDef;
                    if (def) {
                        ++def->RefCount;
                        if (ActivateQuest(tc->ScriptQuestName, true, true)) {
                            {
                                CIntelligentPointer<CThing> pointer(card);
                                ActiveQuestCards.push_back(pointer);
                            }
                            card->SetInLimbo(true);
                            card->SetAsGameStatePersisted(true);
                            World->GuildSealRecallPos = C3DVector(0.0f, 0.0f, 0.0f);
                            World->GuildSealRecallAngleXY = 0.0f;
                            RemoveAvailableQuestCardType(card->GetDefName());
                            g_pPlayerGui->Refresh(0);
                            if (--def->RefCount == 0)
                                def->DeleteThis();
                            return true;
                        }
                        if (--def->RefCount == 0)
                            def->DeleteThis();
                    }
                }
            }
        }
    }
    return false;
}