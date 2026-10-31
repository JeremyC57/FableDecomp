// CQuestManager::SetQuestAsCompleted(const CCharString& name, bool showCompletionScreen, bool autoSave, bool autoSaveArg) -- for a registered quest: post the quest-completed message (and the quest-finished one when no screen is shown); show the hero's completion screen or note the finish screen as displayed; for the quest's active card mark it completed, remember it, bump the hero's quest statistics and pay out its gold, experience, renown and reward items, then drop its card; make the hero's quest-persistent inventories persistent; mark the quest inactive, autosave after a core quest when asked, and clear the checkpoint save
// Grade: DIFFER(1351v1352) (sec1b 2026-10-30) -- body wrapped in `if (index > -1) { ... return true; } return false;`: the return-false epilogue moves to the end as retail (1347 -> 1351, 319 -> 311 difflines). Left: retail keeps name in ebp from the entry (push ebp first), the hero in ebx with a shrink-wrapped save, frame 0x24 vs 0x2c (showCompletionScreen slot reused for the 0xfc key and CWideString temp). Next: a named const CCharString& local for name / block scopes for the late temporaries
//   range abort); register/frame allocation differs: retail keeps name in ebp and the hero in ebx, shrink-wraps the ebx save,
//   moves the return-false epilogue to the end and reuses the dead showCompletionScreen slot for the 0xfc key and the
//   CWideString temp (frame 0x24 vs 0x2c).
// Leaf: SetQuestAsCompleted
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B1D30, 1350 bytes, `ret 0x10`.
// Callees: GetIndexFromRegisteredQuest 0x004AF740, CMessageEventManager::AddMessage 0x00687540, player manager main player
//   0x00449970 / character thing 0x00487DC0, TC map lower_bound 0x0040F020, CTCQuestCompletionUI::Activate 0x005E4740,
//   OnQuestFinishScreenDisplayed 0x004B0160, GetActiveQuestCardFromScriptName 0x004B0C80, CIntelligentPointer ctor
//   0x004A96F0 / dtor 0x00A01C10, completed-card push_back 0x004BA4E0, CTCQuestCard::IsCore 0x00702280, hero stats
//   AddMoney 0x0057B327 / AddExperience 0x0057A59B / AddRenown 0x0057D8BE, CThing::GetPos 0x004C73D0, thing Create
//   0x00703210, GetCategoryIdentifier 0x005D9030, inventory accepts 0x005BF2ED, AddToInventory 0x005D8D50, GetDefName
//   0x004C7CC0, CDefStringTable::GetString 0x009D49B0 (table 0x013CA828), quest-card remove_if 0x004B9250,
//   RemoveActiveQuestCard 0x004B0AA0, SetQuestPersistentFlagForItems 0x005BCF4C, FindActiveQuest 0x004AF5B0,
//   CWorld::AutoSave 0x004A0AE0, DeleteCheckPointSave 0x0049E740, puts/abort (bitset range check).
extern "C" int __cdecl puts(const char* text);
extern "C" void __cdecl abort();
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CDefString { long Index; };
struct CDefStringTable { CCharString GetString(CDefString name); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;            // 0x013CA828
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CTCBase;
struct CTCPair { long first; CTCBase* second; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* _M_end_of_storage;
    CTCPair* lower_bound(const long& key);              // 0x0040F020
};
struct CThing {
    char           Pad0[0x20];
    unsigned long  TCBits[9];                           // +0x20 bitset<0x112>
    CTCMap         TCs;                                 // +0x44
    char           Pad1[0x8c - 0x50];
    unsigned short DefIndex;                            // +0x8c
    char           Pad2[0x91 - 0x8e];
    unsigned char  Flags91;                             // +0x91
    __forceinline unsigned long HasTC(unsigned long type) const
    {
        if (type >= 0x112) {
            puts("bitset");
            abort();
        }
        return TCBits[type >> 5] & (1UL << (type & 31));
    }
    __forceinline CTCBase* GetTC(long type)
    {
        CTCMap& map = TCs;
        long key = type;
        CTCPair* it = map.lower_bound(key);
        CTCPair* end = map._M_finish;
        if (it == end || type < it->first)
            it = end;
        return it->second;
    }
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPos();                          // 0x004C73D0
    CDefString GetDefName();                            // 0x004C7CC0
};
CThing* __fastcall CreateThing(long def, const C3DVector& pos, long type, long a, long b, const CCharString& scriptName);   // 0x00703210
struct CIntelligentPointer {
    void* Data;
    CIntelligentPointer(CThing* thing);                 // 0x004A96F0
    ~CIntelligentPointer();                             // 0x00A01C10
};
struct CRewardVT { virtual void v0(); virtual void Destroy(); };
struct CReward : CRewardVT {
    long  RefCount;                                     // +0x04
    char  Pad[0x4c - 8];
    long* ItemsStart;                                   // +0x4c
    long* ItemsFinish;                                  // +0x50
};
struct CRewardPtr {
    CReward* Data;
    CRewardPtr(CReward* data) : Data(data) { if (Data) ++Data->RefCount; }
    ~CRewardPtr() { if (Data && --Data->RefCount == 0) Data->Destroy(); }
    CReward* operator->() const { return Data; }
};
struct CTCQuestCard {
    char     Pad0[0xc];
    CReward* Reward;                                    // +0x0c
    bool     Completed;                                 // +0x10
    long     Region;                                    // +0x14
    char     Pad1[0x2c - 0x18];
    long     Gold;                                      // +0x2c
    long     Renown;                                    // +0x30
    bool IsCore();                                      // 0x00702280
};
struct CTCHeroStats {
    char Pad[0xbc];
    long QuestsCompleted;                               // +0xbc
    long CoreQuestsCompleted;                           // +0xc0
    long RegionQuestsCompleted;                         // +0xc4
    long OtherQuestsCompleted;                          // +0xc8
    void AddMoney(long amount, bool show);              // 0x0057B327
    void AddExperience(long amount);                    // 0x0057A59B
    void AddRenown(long amount);                        // 0x0057D8BE
};
struct CTCQuestCompletionUI { void Activate(long type, const CCharString& name, bool a, const CWideString& text, bool b); };   // 0x005E4740
struct CTCInventoryBase {
    bool CanAccept(long defIndex);                      // 0x005BF2ED
    void SetQuestPersistentFlagForItems(bool persistent);   // 0x005BCF4C
};
struct CTCInventoryItem {
    unsigned long GetCategoryIdentifier();              // 0x005D9030
    bool AddToInventory(CTCInventoryBase* inventory, bool a, long b, bool c, bool d, bool e);   // 0x005D8D50
};
struct CMessageEventManager { void AddMessage(long type, long a, const C3DVector& pos, long b, long c, long d, long e, long data); };   // 0x00687540
struct CWorld {
    char                  Pad0[0x60];
    CMessageEventManager* MessageManager;               // +0x60
    char                  Pad1[0x140 - 0x64];
    bool                  QuestCompletedThisFrame;      // +0x140
    bool AutoSave(bool arg);                            // 0x004A0AE0
};
struct CPlayer { CThing* GetCharacterThing(); };       // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CMainGameComponent { char Pad[0x1c]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
void __fastcall DeleteCheckPointSave();             // 0x0049E740
struct CActiveQuest { char Pad[0x24]; bool Active; };
struct CIsQuestCardWithDefName { CCharString Name; CIsQuestCardWithDefName(const CCharString& name); };
struct CQuestCardList { void* _M_node; void remove_if(const CCharString& defName); };   // 0x004B9250
struct CCompletedCards { void push_back(const CIntelligentPointer& card); };          // 0x004BA4E0
struct CQuestManager {
    char            Pad0[0x58];
    CQuestCardList  QuestCards;                         // +0x58
    char            Pad1[0x60 - 0x5c];
    CCompletedCards CompletedCards;                     // +0x60
    char            Pad2[0x7c - 0x61];
    CWorld*         World;                              // +0x7c
    char            Pad3[0x8f - 0x80];
    bool            QuestCardsChanged;                  // +0x8f
    long GetIndexFromRegisteredQuest(const CCharString& name);   // 0x004AF740
    bool OnQuestFinishScreenDisplayed(const CCharString& name);  // 0x004B0160
    CThing* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
    void RemoveActiveQuestCard(const CCharString& name);         // 0x004B0AA0
    CActiveQuest* FindActiveQuest(const CCharString& name);      // 0x004AF5B0
    bool SetQuestAsCompleted(const CCharString& name, bool showCompletionScreen, bool autoSave, bool autoSaveArg);
};

bool CQuestManager::SetQuestAsCompleted(const CCharString& name, bool showCompletionScreen, bool autoSave, bool autoSaveArg)
{
    long index = GetIndexFromRegisteredQuest(name);
    if (index > -1) {
    CTCQuestCard* questCard = 0;
    World->MessageManager->AddMessage(0x35, 0x32, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
    if (!showCompletionScreen)
        World->MessageManager->AddMessage(0x33, 0x32, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacterThing();
    if (hero && !hero->IsDead()) {
        if (showCompletionScreen) {
            CTCQuestCompletionUI* ui;
            if (hero->HasTC(0xfc))
                ui = (CTCQuestCompletionUI*)hero->GetTC(0xfc);
            CWideString text((const unsigned short*)L"");
            ui->Activate(1, name, false, text, true);
        }
        else
            OnQuestFinishScreenDisplayed(name);
        CThing* card = GetActiveQuestCardFromScriptName(name);
        if (card) {
            if (card->HasTC(0x6c) && (questCard = (CTCQuestCard*)card->GetTC(0x6c)) != 0) {
                questCard->Completed = true;
                CompletedCards.push_back(CIntelligentPointer(card));
                CTCHeroStats* stats = 0;
                if (hero->HasTC(4))
                    stats = (CTCHeroStats*)hero->GetTC(4);
                ++stats->QuestsCompleted;
                if (questCard->IsCore())
                    ++stats->CoreQuestsCompleted;
                else if (questCard->Region > -1)
                    ++stats->RegionQuestsCompleted;
                else
                    ++stats->OtherQuestsCompleted;
                if (CRewardPtr(questCard->Reward).Data != 0) {
                    stats->AddMoney(questCard->Gold, true);
                    stats->AddExperience(questCard->Gold);
                    stats->AddRenown(questCard->Renown);
                    CTCInventoryBase* inventory;
                    for (unsigned long i = 0; i < (unsigned long)(CRewardPtr(questCard->Reward)->ItemsFinish - CRewardPtr(questCard->Reward)->ItemsStart); ++i) {
                        CThing* item = CreateThing(CRewardPtr(questCard->Reward)->ItemsStart[i], hero->GetPos(), 4, 0, 0, CCharString(""));
                        if (item && item->HasTC(0x1a)) {
                            CTCInventoryItem* inventoryItem = (CTCInventoryItem*)item->GetTC(0x1a);
                            if (inventoryItem) {
                                unsigned long category = inventoryItem->GetCategoryIdentifier();
                                if (hero->HasTC(category))
                                    inventory = (CTCInventoryBase*)hero->GetTC(category);
                                if (inventory->CanAccept(item->DefIndex))
                                    inventoryItem->AddToInventory(inventory, false, 1, false, true, false);
                            }
                        }
                    }
                }
            }
            QuestCards.remove_if(g_DefStringTable.GetString(card->GetDefName()));
            QuestCardsChanged = true;
            RemoveActiveQuestCard(name);
        }
        if (hero->HasTC(0x11))
            ((CTCInventoryBase*)hero->GetTC(0x11))->SetQuestPersistentFlagForItems(true);
        if (hero->HasTC(0x13))
            ((CTCInventoryBase*)hero->GetTC(0x13))->SetQuestPersistentFlagForItems(true);
        if (hero->HasTC(0x12))
            ((CTCInventoryBase*)hero->GetTC(0x12))->SetQuestPersistentFlagForItems(true);
    }
    CActiveQuest* quest = FindActiveQuest(name);
    if (quest)
        quest->Active = false;
    if (autoSave && questCard && questCard->IsCore())
        World->AutoSave(autoSaveArg);
    DeleteCheckPointSave();
    World->QuestCompletedThisFrame = false;
    return true;
    }
    return false;
}
