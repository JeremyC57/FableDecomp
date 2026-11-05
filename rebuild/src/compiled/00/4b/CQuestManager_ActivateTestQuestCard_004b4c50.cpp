// CQuestManager::ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName) -- debug: create the named quest-card object at the living hero, point its quest card at the given script quest and activate it (loading resources); on success the card becomes active (in limbo, persisted), the guild-seal recall point is reset, its type leaves the available cards and the player GUI refreshes
// Leaf: ActivateTestQuestCard
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
class CDefString { public:                             // naming wave 34: was N_00415dd0::CDefString::operator CCharString (as 0x004B4AA0, Codex pass 12)
    long m_index;
    CCharString GetString() const;                       // 0x00415DD0 (FableWin ?GetString@CDefString@@QBE?AVCCharString@@XZ)
};
#include "engine/C3DVector.h"

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
    __forceinline CTCBase* GetTC(long type)
    {
        CTCMap* map = &Components;
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->first)
            it = map->_M_finish;
        return it->second;
    }
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    void SetAsGameStatePersisted(bool persisted);        // 0x004C7880
    CDefString GetDefName() const;                       // 0x004C7CC0 (Ego_r / FableWin ?GetDefName@CThing@@QBE?AVCDefString@@XZ)
    const C3DVector& GetPos() const;                // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };

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
class CRightHandedSet;
class CThingObject;
class CThingObject { public:
    static CThingObject* __fastcall Create(long a0, C3DVector const&a1, long a2, CRightHandedSet const*a3, unsigned long a4, CCharString const&a5);   // 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};
class CPlayer { public:
    unsigned char _pad00[0x28];
    long          PlayerNumber;                          // +0x28
    CThingPlayerCreature* GetPControlledCreature() const;                           // 0x00487DC0
    long GetPlayerNumber() const { return PlayerNumber; }
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                       // +0x1c
    CPlayerManager* GetPlayerManager() const { return PlayerManager; }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);                // 0x004378C0
};

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
    CPlayer* player = &CMainGameComponent::Get()->GetPlayerManager()->GetMainPlayer();
    CThing* hero = player->GetPControlledCreature();
    if (hero && !hero->Dead) {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(cardDefName);
        if (defIndex > 0) {
            CThing* card = ((CThing*)CThingObject::Create(defIndex, hero->GetPos(), player->GetPlayerNumber(), 0, 0, ""));
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
                            {
                                // the def name is a named local (as the sibling ActivateQuestCard 0x004B4AA0)
                                CCharString defName = card->GetDefName().GetString();
                                RemoveAvailableQuestCardType(defName);
                            }
                            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
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