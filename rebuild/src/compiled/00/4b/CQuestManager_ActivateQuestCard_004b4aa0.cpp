// CQuestManager::ActivateQuestCard(CThing* card, bool loadResources) -- clear the objectives; if the living card thing's quest card is activatable, has a def, no quest is between its start and finish screens in the card's region and its script quest activates, make the card active (in limbo, persisted), reset the guild-seal recall point, drop the card's type from the available cards and refresh the player GUI
// Leaf: ActivateQuestCard
// Shape: the quest-card component lookup is a template-constant-key GetTC: LowerBound binds its const long& to the constant, so the
//   key temporary is homed in the dead `card` argument slot (shared with the uninitialised-on-miss tc) and the compare stays an
//   immediate; a by-value key copy local takes its own frame word.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020 (the non-const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                              // +0x91 bit 0
    unsigned char _91_1 : 7;
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <long K> __forceinline CTCBase* GetTCK()
    {
        CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound((ETCInterfaceType)K);
        if (it == map->_M_finish || K < it->first)
            it = map->_M_finish;
        return it->second;
    }
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    void SetAsGameStatePersisted(bool persisted);        // 0x004C7880
    CDefString GetDefName();                             // 0x004C7CC0
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                        // 0x004A96F0 (T = CThing)

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
    bool ActivateQuestCard(CThing* card, bool loadResources);
};

bool CQuestManager::ActivateQuestCard(CThing* card, bool loadResources)
{
    Objectives.clear();
    if (card && !card->Dead) {
        CTCQuestCard* tc;
        if (card->HasTC(0x6c))
            tc = (CTCQuestCard*)card->GetTCK<0x6c>();
        if (tc->IsActivatable_00702810()) {
            CQuestCardDef* def = tc->PDef.PDef;
            if (def) {
                ++def->RefCount;
                if (!IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(tc->GetRegionName())) {
                    if (ActivateQuest(tc->ScriptQuestName, true, loadResources)) {
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
                }
                if (--def->RefCount == 0)
                    def->DeleteThis();
            }
        }
    }
    return false;
}