// CGameScriptInterface::GiveHeroQuestCardDirectly(const CCharString& cardDefName, const CCharString& questName, bool silent) const -- create the named quest-card object at the hero, name its quest and activate it; tell the hero through the guild master unless silent or a vignette; kill the card when it cannot be activated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008968C0, length 364 bytes (`ret 0Ch`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
//   CQuestManager::IsQuestCardActive(const CCharString&) @ 0x004AF560 and ActivateQuestCard(CThing*, bool) @ 0x004B4AA0
//   on g_pQuestManager (0x013B89FC), CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall),
//   CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPos @ 0x004C73D0,
//   CThingObject::Create @ 0x00703210 (static __fastcall), the component map LowerBound @ 0x0040F020,
//   CCharString ctor/dtor/operator= @ 0x0099EBF0 / 0x0099EAE0 / 0x0099EFB0, CTCQuestCard::IsVignette @ 0x007022E0,
//   CThing::Kill(bool) @ 0x004C9B80, own virtual slot 274 HeroReceiveMessageFromGuildMaster (0x008918C0).
// Layout: CThing player byte +0x90 (signed), dead flags +0x91 (bit 0), UsingComponents +0x20 (quest card = TC 0x6C,
//   word 3 bit 12), component map +0x44 (the landed 0x00896A30 / 0x006223C0 model); CTCQuestCard quest name +0x28.
// Shape: the scratch strings live in dead argument slots (cardDefName after the index lookup, silent after its test);
//   the guild-master message's two literals are temporaries built right to left.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((ETCInterfaceType)kind);
        return ((it == m_pLast || kind < it->first) ? m_pLast : it)->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                         // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                               // +0x44
    unsigned char  _pad50[0x40];
    signed char    m_player;                            // +0x90
    unsigned char  m_deadFlags;                         // +0x91
    const C3DVector& GetPos() const;               // 0x004C73D0
    void Kill(bool silent);                             // 0x004C9B80
};
class CThingPlayerCreature : public CThing { };
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
class CTCQuestCard { public:
    unsigned char _pad00[0x28];
    CCharString QuestName;                              // +0x28
    bool IsVignette() const;                            // 0x007022E0
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };             // 0x00487DC0
class CPlayerManager { public: public:
    CPlayer& GetMainPlayer();   // 0x00449970
};    // 0x00449970
class CQuestManager { public:
    bool IsQuestCardActive(const CCharString& name);    // 0x004AF560
    bool ActivateQuestCard(CThing* card, bool b);       // 0x004B4AA0
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};

#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD256(pa) VPAD16(pb) VPAD2(pc)                    // slots 0..273
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& message, const CCharString& orb,
                                                   bool immediate, bool b) const;   // slot 274
    virtual void GiveHeroQuestCardDirectly(const CCharString& cardDefName, const CCharString& questName, bool silent) const;
    void*            World;                             // +0x04
    void*            Component;                         // +0x08
    void*            DisplayEngine;                     // +0x0c
    void*            DefinitionManager;                 // +0x10
    CPlayerManager*  PlayerManager;                     // +0x14
};

void CGameScriptInterface::GiveHeroQuestCardDirectly(const CCharString& cardDefName, const CCharString& questName, bool silent) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || (hero->m_deadFlags & 1))
        return;
    if (g_pQuestManager->IsQuestCardActive(cardDefName))
        return;
    long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(cardDefName);
    if (index <= 0)
        return;
    CThingObject* card;
    {
        CCharString scriptName("");
        card = CThingObject::Create(index, hero->GetPos(), hero->m_player, 0, 0, scriptName);
    }
    if (!card)
        return;
    if (card->m_tcMask[3] & 0x1000) {
        CTCQuestCard* tc = (CTCQuestCard*)card->m_tcs.Find(0x6c);
        if (tc) {
            tc->QuestName = questName;
            if (g_pQuestManager->ActivateQuestCard(card, true)) {
                if (!silent && !tc->IsVignette()) {
                    CCharString orb("HUD_ORB_QUEST_CORE");
                    CCharString message("TEXT_QST_078_GM_MSG_NEW_QUEST_CARD");
                    HeroReceiveMessageFromGuildMaster(message, orb, true, false);
                }
                return;
            }
        }
    }
    card->Kill(false);
}