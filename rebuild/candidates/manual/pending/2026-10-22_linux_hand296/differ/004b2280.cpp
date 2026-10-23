// Grade: DIFFER(527v527) -- register / frame-slot colouring only: retail keeps questName in edi and `this` in ebp
//   (ours: ebp / edi; the card later reuses questName's register in both), and homes the 0x6C / 4 interface keys and
//   out-pointers in the dead showFailureScreen argument slot ([esp+24h]) where ours uses the dead `text` slot
//   ([esp+2Ch]). Structure, size and every call match (forceinline message helper with the zero position built after
//   the radius push; `return false` tail last). Tried: card declared at the top, a reference local for the name, a
//   const self pointer, out-pointers at hero-block / function scope, the KickOffDeathScreen GetInterface form (535),
//   QFE.
// CQuestManager::SetQuestAsFailed(const CCharString& questName, bool showFailureScreen, const CWideString& title, const CWideString& text) -- for a registered quest post the "quest failed" message (and "quest finished" when no screen is shown); for a living hero show the failure screen (else run the finish-screen bookkeeping), mark the quest's active card failed, count the failure, offer its card type again and retire the card; false for an unknown quest
// Leaf: SetQuestAsFailed
// Retail 0x004B2280, 528 bytes, `ret 0x10` (Ego_r.pdb ?SetQuestAsFailed@CQuestManager@@QAE_NABVCCharString@@_NABVCWideString@@1@Z).
// Callees: GetIndexFromRegisteredQuest 0x004AF740, CMessageEventManager::AddMessage 0x00687540 (World +0x60; types 0x36
// and 0x34, radius 50), CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroCreature 0x00487DC0,
// CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: 0xFC CTCQuestCompletionUI, 0x6C CTCQuestCard, 4 the hero
// stats), CTCQuestCompletionUI::Activate(CompletionType, const CCharString&, bool, const CWideString&, const
// CWideString&) 0x005E4740, OnQuestFinishScreenDisplayed 0x004B0160, GetActiveQuestCardFromScriptName 0x004B0C80,
// CThing::GetDefName 0x004C7CC0, CDefStringTable::GetString 0x009D49B0 (g_DefStringTable 0x013CA828), the card-type
// list's add 0x004B9250 (name unconfirmed), CCharString dtor 0x0099EAE0, RemoveActiveQuestCardAndMakeAvailableAgain
// 0x004B0B90.
// Layout: retail STLport CQuestManager (AvailableQuestCardTypes +0x58, World +0x7c, QuestCardsNeedUpdating +0x8f);
// CThing presence bits +0x20 / interface map +0x44 / Dead +0x91 bit 0; CTCQuestCard Failed +0x10; hero stats +0xb8.
// Shape: the zero position is built inside an inline CQuestManager message helper (stores after the radius push, as in
// the PostBoastTransfer sibling); each interface fetch is the inline GetInterface whose out pointer stays unset when
// the interface is absent (retail then reads the uninitialised slot).
struct CCharString {
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
struct CWideString { void* PStringData; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long distance, const C3DVector& pos, CThing* source, CThing* extra, float a,
                     float b, long data);           // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessages;              // +0x60
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CDefString { long Index; };
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    CDefString GetDefName();                        // 0x004C7CC0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CDefStringTable { CCharString GetString(CDefString name); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;            // 0x013CA828
struct CTCQuestCompletionUI {                       // interface 0xFC
    void Activate(long type, const CCharString& questName, bool b, const CWideString& title,
                  const CWideString& text);         // 0x005E4740
};
struct CTCQuestCard {                               // interface 0x6C
    unsigned char _pad[0x10];
    bool Failed;                                    // +0x10
};
struct CTCHeroStats {                               // interface 4
    unsigned char _pad[0xb8];
    long QuestsFailed;                              // +0xb8
};
struct CPlayer { CThing* GetHeroCreature(); };      // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CEngine {
    unsigned char _pad00[0x1c];
    CPlayerManager* PPlayerManager;                 // +0x1c
};
extern CEngine* g_pEngine;                          // 0x013B86A0
struct CQuestCardTypeList {                         // list at +0x58
    CQuestCardTypeList* m_node;
    void Add(const CCharString& type);              // 0x004B9250
};

struct CQuestManager {
    unsigned char      _pad00[0x58];
    CQuestCardTypeList AvailableQuestCardTypes;     // +0x58
    unsigned char      _pad5c[0x7c - 0x5c];
    CWorld*            World;                       // +0x7c
    unsigned char      _pad80[0x8f - 0x80];
    bool               QuestCardsNeedUpdating;      // +0x8f

    long GetIndexFromRegisteredQuest(const CCharString& name);                          // 0x004AF740
    bool OnQuestFinishScreenDisplayed(const CCharString& name);                         // 0x004B0160
    CThing* GetActiveQuestCardFromScriptName(const CCharString& name) const;            // 0x004B0C80
    void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString& name, bool b);   // 0x004B0B90
    __forceinline void PostMessage(long type, long data)
    {
        World->m_pMessages->AddMessage(type, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, data);
    }
    bool SetQuestAsFailed(const CCharString& questName, bool showFailureScreen, const CWideString& title,
                          const CWideString& text);
};

bool CQuestManager::SetQuestAsFailed(const CCharString& questName, bool showFailureScreen, const CWideString& title,
                                     const CWideString& text)
{
    long index = GetIndexFromRegisteredQuest(questName);
    if (index > -1) {
        PostMessage(0x36, index);
        if (!showFailureScreen)
            PostMessage(0x34, index);
        CThing* hero = g_pEngine->PPlayerManager->GetMainPlayer()->GetHeroCreature();
        if (hero && !hero->Dead) {
            if (showFailureScreen) {
                CTCQuestCompletionUI* ui;
                hero->GetInterface(&ui, 0xfc);
                ui->Activate(2, questName, false, title, text);
            } else {
                OnQuestFinishScreenDisplayed(questName);
            }
            CThing* card = GetActiveQuestCardFromScriptName(questName);
            if (card) {
                CTCQuestCard* questCard;
                card->GetInterface(&questCard, 0x6c);
                if (questCard)
                    questCard->Failed = true;
                CTCHeroStats* stats;
                hero->GetInterface(&stats, 4);
                ++stats->QuestsFailed;
                CCharString cardType = g_DefStringTable.GetString(card->GetDefName());
                AvailableQuestCardTypes.Add(cardType);
                QuestCardsNeedUpdating = true;
            }
            RemoveActiveQuestCardAndMakeAvailableAgain(questName, false);
        }
        return true;
    }
    return false;
}
