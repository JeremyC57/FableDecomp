// Standalone cases for the selected quest source, with dependency doubles.
#include <stdio.h>
#include <string>
#include <vector>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/EMessageEventType.h"
#include "engine/ETCInterfaceType.h"
#include "engine/ThingAlive.h"
#include "fable_assert.h"

typedef std::string CCharString;
typedef std::wstring CWideString;

class CThing;
class CTCQuestCompletionUI;
class CTCHeroStats;
class CTCQuestCard;
class CTCInventoryItem;

namespace NInventory {
class CTCInventoryBase {
public:
    explicit CTCInventoryBase(bool hasSpace = true) : PersistentCalls(0), SpaceCalls(0),
        LastDef(-1), HasSpace(hasSpace) {}
    bool IsSpaceInInventoryForItem(long def)
    {
        ++SpaceCalls;
        LastDef = def;
        return HasSpace;
    }
    void SetQuestPersistFlagForAllItems(bool value)
    {
        if (value)
            ++PersistentCalls;
    }
    unsigned PersistentCalls, SpaceCalls;
    long LastDef;
    bool HasSpace;
};
}

class CTCInventory : public NInventory::CTCInventoryBase {};
class CTCInventoryWeapons : public NInventory::CTCInventoryBase {};
class CTCInventoryClothing : public NInventory::CTCInventoryBase {};

class CThing {
public:
    CThing() : Dead(false), Definition(0), Pos(1, 2, 3), DefName("card-type"),
        QuestCard(0), InventoryItem(0), HasInventoryItem(false) {}
    bool IsDead() const { return Dead; }
    unsigned long GetDefGlobalIndex() const { return Definition; }
    const C3DVector& GetPos() const { return Pos; }
    const CCharString& GetDefName() const { return DefName; }
    bool GetTC(CTCQuestCard** result)
    {
        if (QuestCard)
            *result = QuestCard;
        return QuestCard != 0;
    }
    bool GetTC(CTCInventoryItem** result)
    {
        if (HasInventoryItem)
            *result = InventoryItem;
        return HasInventoryItem;
    }
    bool Dead;
    unsigned long Definition;
    C3DVector Pos;
    CCharString DefName;
    CTCQuestCard* QuestCard;
    CTCInventoryItem* InventoryItem;
    bool HasInventoryItem;
};

class CThingPlayerCreature : public CThing {
public:
    CThingPlayerCreature() : Ui(0), Stats(0), Inventory(0), Weapons(0), Clothing(0),
        DynamicInventory(0), LastInventoryType(TCI_INVENTORY) {}
    bool GetTC(CTCQuestCompletionUI** result) { return Report(Ui, result); }
    bool GetTC(CTCHeroStats** result) { return Report(Stats, result); }
    bool GetTC(CTCInventory** result) { return Report(Inventory, result); }
    bool GetTC(CTCInventoryWeapons** result) { return Report(Weapons, result); }
    bool GetTC(CTCInventoryClothing** result) { return Report(Clothing, result); }
    bool GetTC(ETCInterfaceType type, NInventory::CTCInventoryBase** result)
    {
        LastInventoryType = type;
        return Report(DynamicInventory, result);
    }
    CTCQuestCompletionUI* Ui;
    CTCHeroStats* Stats;
    CTCInventory* Inventory;
    CTCInventoryWeapons* Weapons;
    CTCInventoryClothing* Clothing;
    NInventory::CTCInventoryBase* DynamicInventory;
    ETCInterfaceType LastInventoryType;
private:
    template<class T> static bool Report(T* value, T** result)
    {
        if (value)
            *result = value;
        return value != 0;
    }
};

class CThingObject : public CThing {
public:
    struct Spawn {
        long Definition;
        CThingObject* Result;
        Spawn(long def, CThingObject* result) : Definition(def), Result(result) {}
    };
    static CThingObject* Create(long def, const C3DVector& pos, long player,
                                const void* orientation, unsigned long flags, const CCharString& script)
    {
        if (Calls >= Plan.size() || def != Plan[Calls].Definition || pos.X != 1 ||
            pos.Y != 2 || pos.Z != 3 || player != 4 || orientation != 0 || flags != 0 ||
            !script.empty()) {
            Correct = false;
            return 0;
        }
        return Plan[Calls++].Result;
    }
    static std::vector<Spawn> Plan;
    static unsigned Calls;
    static bool Correct;
};

std::vector<CThingObject::Spawn> CThingObject::Plan;
unsigned CThingObject::Calls;
bool CThingObject::Correct;

class CTCQuestCompletionUI {
public:
    enum CompletionType { QUEST_SUCCESS = 1 };
    CTCQuestCompletionUI() : Calls(0), Correct(false) {}
    void Activate(CompletionType type, const CCharString& script, bool a,
                  const CWideString& summary, bool b)
    {
        ++Calls;
        Correct = type == QUEST_SUCCESS && script == "quest" && !a && summary.empty() && b;
    }
    unsigned Calls;
    bool Correct;
};

class CTCHeroStats {
public:
    CTCHeroStats() : Quests(0), Core(0), Feat(0), Optional(0), Gold(0), RewardGold(0), Renown(0),
        CorrectGoldDisplay(false) {}
    long GetNumCompletedAQuest() const { return Quests; }
    long GetNumCompletedACoreQuest() const { return Core; }
    long GetNumCompletedAFeatQuest() const { return Feat; }
    long GetNumCompletedAnOptionalQuest() const { return Optional; }
    void SetNumCompletedAQuest(long n) { Quests = n; }
    void SetNumCompletedACoreQuest(long n) { Core = n; }
    void SetNumCompletedAFeatQuest(long n) { Feat = n; }
    void SetNumCompletedAnOptionalQuest(long n) { Optional = n; }
    void AddMoney(long n, bool display) { Gold += n; CorrectGoldDisplay = display; }
    void AddRewardMoney(long n) { RewardGold += n; }
    void AddRenown(long n) { Renown += n; }
    long Quests, Core, Feat, Optional, Gold, RewardGold, Renown;
    bool CorrectGoldDisplay;
};

class CQuestCardDef {
public:
    std::vector<long> RewardObjects;
};

template<class T> class CDefPointer {
public:
    explicit CDefPointer(T* item) : Item(item) {}
    bool IsNull() const { return Item == 0; }
    T* operator->() const { return Item; }
private:
    T* Item;
};

class CTCQuestCard {
public:
    CTCQuestCard() : Finished(false), Core(false), Feat(false), Def(0), CoreCalls(0) {}
    void SetAsFinished(bool finished) { Finished = finished; }
    bool IsCore() const { ++CoreCalls; return Core; }
    bool IsFeat() const { return Feat; }
    CDefPointer<const CQuestCardDef> GetPDef() const { return CDefPointer<const CQuestCardDef>(Def); }
    long GetGoldReward() const { return 19; }
    long GetRenownReward() const { return 5; }
    bool Finished, Core, Feat;
    CQuestCardDef* Def;
    mutable unsigned CoreCalls;
};

class CTCInventoryItem {
public:
    CTCInventoryItem() : Calls(0), Correct(false), ReceivedInventory(0) {}
    ETCInterfaceType GetInventoryType() const { return TCI_INVENTORY_WEAPONS; }
    bool AddToInventory(NInventory::CTCInventoryBase& inventory, bool a, bool b, long c, bool d, bool e)
    {
        ++Calls;
        Correct = !a && b && c == 0 && d && !e;
        ReceivedInventory = &inventory;
        return true;
    }
    unsigned Calls;
    bool Correct;
    NInventory::CTCInventoryBase* ReceivedInventory;
};

class CMessageEventManager {
public:
    CMessageEventManager() : Correct(true), ExpectedIndex(7) {}
    void AddMessage(EMessageEventType type, unsigned long lifetime, const C3DVector& pos,
                    CThing* first, CThing* second, float a, float b, long index)
    {
        Types.push_back(type);
        Correct = Correct && lifetime == 50 && pos.X == 0 && pos.Y == 0 && pos.Z == 0 &&
                  !first && !second && a == 0 && b == 0 && index == ExpectedIndex;
    }
    std::vector<EMessageEventType> Types;
    bool Correct;
    long ExpectedIndex;
};

class CWorld {
public:
    CWorld() : Saves(0), ForceSave(false), CheckpointExists(true) {}
    CMessageEventManager& GetMessageEventManager() { return Messages; }
    bool AutoSave(bool force) { ++Saves; ForceSave = force; return true; }
    static void DeleteCheckPointSave() { ++Deletes; }
    void SetAutoSaveCheckPointExists(bool exists) { CheckpointExists = exists; }
    CMessageEventManager Messages;
    unsigned Saves;
    bool ForceSave, CheckpointExists;
    static unsigned Deletes;
};

unsigned CWorld::Deletes;

class CPlayer {
public:
    CPlayer() : Hero(0) {}
    CThingPlayerCreature* GetPControlledCreature() const { return Hero; }
    CThingPlayerCreature* Hero;
};

class CPlayerManager {
public:
    CPlayer& GetMainPlayer() { return Player; }
    CPlayer Player;
};

class CMainGameComponent {
public:
    static CMainGameComponent* Get() { return Instance; }
    CPlayerManager& GetPlayerManager() { return Players; }
    CPlayerManager Players;
    static CMainGameComponent* Instance;
};

CMainGameComponent* CMainGameComponent::Instance;

template<class T> class CIntelligentPointer {
public:
    CIntelligentPointer(T* item) : Item(item) {}
    T* Get() const { return Item; }
private:
    T* Item;
};

class CQuestManager {
public:
    class CActiveQuest {
    public:
        CActiveQuest() : Persistent(true) {}
        void SetPersistent(bool persistent) { Persistent = persistent; }
        bool Persistent;
    };

    explicit CQuestManager(CWorld& world) : World(world), Registration(7), Card(0), Quest(0),
        FinishScreenCalls(0), CorrectScript(true) {}

    bool SetQuestAsCompleted(const CCharString&, bool, bool, bool);
    long GetIndexFromRegisteredQuest(const CCharString& script) { CheckScript(script); return Registration; }
    void OnQuestFinishScreenDisplayed(const CCharString& script) { CheckScript(script); ++FinishScreenCalls; }
    CThing* GetActiveQuestCardFromScriptName(const CCharString& script) { CheckScript(script); return Card; }
    void RemoveAvailableQuestCardType(const CCharString& def) { RemovedType = def; }
    void RemoveActiveQuestCard(const CCharString& script) { CheckScript(script); Card = 0; }
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& script) { CheckScript(script); return Quest; }
    void CheckScript(const CCharString& script) { CorrectScript = CorrectScript && script == "quest"; }

    CWorld& World;
    long Registration;
    CThing* Card;
    CActiveQuest* Quest;
    unsigned FinishScreenCalls;
    bool CorrectScript;
    CCharString RemovedType;
    std::vector<CIntelligentPointer<CThing> > CompletedQuestCards;
};

bool CQuestManager::SetQuestAsCompleted(const CCharString& script_name, bool display_screen,
                                       bool auto_save, bool force_regular_auto_save)
{
    long index = GetIndexFromRegisteredQuest(script_name);
    if (index > -1) {
        CTCQuestCard* ptc_quest_card = 0;
        World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED_BEFORE_SCREEN_SHOWN,
                                                  50, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
        if (!display_screen)
            World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED,
                                                      50, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
        CThingPlayerCreature* phero =
            CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            if (display_screen) {
                CTCQuestCompletionUI* ptc_ui;
                if (!phero->GetTC(&ptc_ui))
                    FABLE_RETAIL_ASSERT_ARM(false);
                ptc_ui->Activate(CTCQuestCompletionUI::QUEST_SUCCESS, script_name, false, L"", true);
            } else {
                OnQuestFinishScreenDisplayed(script_name);
            }

            CThing* pquest_card = GetActiveQuestCardFromScriptName(script_name);
            if (pquest_card) {
                if (!pquest_card->GetTC(&ptc_quest_card))
                    FABLE_RETAIL_ASSERT_ARM(false);
                if (ptc_quest_card) {
                    ptc_quest_card->SetAsFinished(true);
                    CompletedQuestCards.push_back(pquest_card);
                    CTCHeroStats* ptc_hero_stats = 0;
                    if (!phero->GetTC(&ptc_hero_stats))
                        FABLE_RETAIL_ASSERT_ARM(false);
                    ptc_hero_stats->SetNumCompletedAQuest(ptc_hero_stats->GetNumCompletedAQuest() + 1);
                    if (ptc_quest_card->IsCore())
                        ptc_hero_stats->SetNumCompletedACoreQuest(ptc_hero_stats->GetNumCompletedACoreQuest() + 1);
                    else if (ptc_quest_card->IsFeat())
                        ptc_hero_stats->SetNumCompletedAFeatQuest(ptc_hero_stats->GetNumCompletedAFeatQuest() + 1);
                    else
                        ptc_hero_stats->SetNumCompletedAnOptionalQuest(ptc_hero_stats->GetNumCompletedAnOptionalQuest() + 1);

                    if (!ptc_quest_card->GetPDef().IsNull()) {
                        ptc_hero_stats->AddMoney(ptc_quest_card->GetGoldReward(), true);
                        ptc_hero_stats->AddRewardMoney(ptc_quest_card->GetGoldReward());
                        ptc_hero_stats->AddRenown(ptc_quest_card->GetRenownReward());
                        for (unsigned long count = 0; count < ptc_quest_card->GetPDef()->RewardObjects.size(); count++) {
                            CThing* pobject = CThingObject::Create(ptc_quest_card->GetPDef()->RewardObjects[count],
                                                                  phero->GetPos(), 4, 0, 0, "");
                            if (pobject) {
                                CTCInventoryItem* ptc_item = 0;
                                if (!pobject->GetTC(&ptc_item))
                                    FABLE_RETAIL_ASSERT_ARM(false);
                                if (ptc_item) {
                                    NInventory::CTCInventoryBase* ptc_inventory;
                                    if (!phero->GetTC(ptc_item->GetInventoryType(), &ptc_inventory))
                                        FABLE_RETAIL_ASSERT_ARM(false);
                                    if (ptc_inventory->IsSpaceInInventoryForItem(pobject->GetDefGlobalIndex()))
                                        ptc_item->AddToInventory(*ptc_inventory, false, true, 0, true, false);
                                }
                            }
                        }
                    }
                }

                RemoveAvailableQuestCardType(pquest_card->GetDefName());
                RemoveActiveQuestCard(script_name);
            }

            CTCInventory* ptc_inventory;
            if (phero->GetTC(&ptc_inventory))
                ptc_inventory->SetQuestPersistFlagForAllItems(true);
            CTCInventoryWeapons* ptc_inventory_weapons;
            if (phero->GetTC(&ptc_inventory_weapons))
                ptc_inventory_weapons->SetQuestPersistFlagForAllItems(true);
            CTCInventoryClothing* ptc_inventory_clothing;
            if (phero->GetTC(&ptc_inventory_clothing))
                ptc_inventory_clothing->SetQuestPersistFlagForAllItems(true);
        }

        CActiveQuest* pquest = GetActiveQuestFromScriptName(script_name);
        if (pquest)
            pquest->SetPersistent(false);
        if (auto_save && ptc_quest_card && ptc_quest_card->IsCore())
            World.AutoSave(force_regular_auto_save);
        CWorld::DeleteCheckPointSave();
        World.SetAutoSaveCheckPointExists(false);
        return true;
    }
    return false;
}

struct Scenario {
    Scenario() : Manager(World)
    {
        CMainGameComponent::Instance = &Game;
        CWorld::Deletes = 0;
        CThingObject::Plan.clear();
        CThingObject::Calls = 0;
        CThingObject::Correct = true;
        Game.Players.Player.Hero = &Hero;
        Hero.Ui = &Screen;
        Hero.Stats = &Stats;
        Hero.Inventory = &Inventory;
        Hero.Weapons = &Weapons;
        Hero.Clothing = &Clothing;
        Hero.DynamicInventory = &Weapons;
        Manager.Card = &QuestThing;
        QuestThing.QuestCard = &QuestCard;
        Manager.Quest = &ActiveQuest;
    }

    bool Complete(bool show = false, bool save = false, bool force = false)
    {
        return Manager.SetQuestAsCompleted("quest", show, save, force);
    }

    bool CheckMessages(bool show) const
    {
        const CMessageEventManager& messages = World.Messages;
        return messages.Correct && messages.Types.size() == (show ? 1U : 2U) &&
               messages.Types[0] == MESSAGE_EVENT_QUEST_COMPLETED_BEFORE_SCREEN_SHOWN &&
               (show || messages.Types[1] == MESSAGE_EVENT_QUEST_COMPLETED) && Manager.CorrectScript;
    }

    bool CheckFinish() const
    {
        return !World.CheckpointExists && CWorld::Deletes == 1 && !ActiveQuest.Persistent;
    }

    CMainGameComponent Game;
    CWorld World;
    CQuestManager Manager;
    CThingPlayerCreature Hero;
    CTCQuestCompletionUI Screen;
    CTCHeroStats Stats;
    CTCInventory Inventory;
    CTCInventoryWeapons Weapons;
    CTCInventoryClothing Clothing;
    CThing QuestThing;
    CTCQuestCard QuestCard;
    CQuestCardDef Definition;
    CQuestManager::CActiveQuest ActiveQuest;
};

static bool RejectedQuest()
{
    Scenario test;
    test.Manager.Registration = -1;
    return !test.Complete(true, true, true) && test.World.Messages.Types.empty() &&
           test.World.CheckpointExists && test.ActiveQuest.Persistent && CWorld::Deletes == 0 &&
           test.Manager.Card == &test.QuestThing;
}

static bool HeroUnavailable(bool missing)
{
    Scenario test;
    if (missing)
        test.Game.Players.Player.Hero = 0;
    else
        test.Hero.Dead = true;
    return test.Complete(true, true, true) && test.CheckMessages(true) && test.CheckFinish() &&
           test.Screen.Calls == 0 && test.Manager.Card == &test.QuestThing && test.World.Saves == 0 &&
           test.Inventory.PersistentCalls == 0 && test.Weapons.PersistentCalls == 0 &&
           test.Clothing.PersistentCalls == 0;
}

static bool MissingCard(bool missingComponent)
{
    Scenario test;
    if (missingComponent)
        test.QuestThing.QuestCard = 0;
    else
        test.Manager.Card = 0;
    test.Hero.Inventory = 0;
    test.Hero.Clothing = 0;
    test.Manager.Quest = 0;

    return test.Complete(false, true, false) && test.CheckMessages(false) &&
           test.Manager.FinishScreenCalls == 1 && test.Manager.Card == 0 &&
           test.Manager.CompletedQuestCards.empty() && test.World.Saves == 0 &&
           test.Inventory.PersistentCalls == 0 && test.Weapons.PersistentCalls == 1 &&
           test.Clothing.PersistentCalls == 0 && !test.World.CheckpointExists && CWorld::Deletes == 1 &&
           test.Manager.RemovedType == (missingComponent ? "card-type" : "");
}

static bool CheckQuestClass(bool core, bool feat, bool save, bool force)
{
    Scenario test;
    test.QuestCard.Core = core;
    test.QuestCard.Feat = feat;
    bool saves = save && core;
    return test.Complete(true, save, force) && test.CheckMessages(true) && test.CheckFinish() &&
           test.Screen.Calls == 1 && test.Screen.Correct && test.Manager.FinishScreenCalls == 0 &&
           test.QuestCard.Finished && test.Manager.Card == 0 && test.Manager.RemovedType == "card-type" &&
           test.Manager.CompletedQuestCards.size() == 1 &&
           test.Manager.CompletedQuestCards[0].Get() == &test.QuestThing &&
           test.Stats.Quests == 1 && test.Stats.Core == (core ? 1 : 0) &&
           test.Stats.Feat == (!core && feat ? 1 : 0) && test.Stats.Optional == (!core && !feat ? 1 : 0) &&
           test.Stats.Gold == 0 && test.Stats.RewardGold == 0 && test.Stats.Renown == 0 &&
           test.Inventory.PersistentCalls == 1 && test.Weapons.PersistentCalls == 1 &&
           test.Clothing.PersistentCalls == 1 && test.World.Saves == (saves ? 1U : 0U) &&
           (!saves || test.World.ForceSave == force) && test.QuestCard.CoreCalls == (save ? 2U : 1U);
}

static bool Rewards(bool full, bool empty)
{
    Scenario test;
    test.QuestCard.Def = &test.Definition;
    test.Weapons.HasSpace = !full;
    CThingObject missingTC;
    CThingObject nullTC;
    nullTC.HasInventoryItem = true;
    CThingObject success;
    CTCInventoryItem item;
    success.Definition = 24;
    success.HasInventoryItem = true;
    success.InventoryItem = &item;
    if (!empty) {
        test.Definition.RewardObjects.push_back(21);
        test.Definition.RewardObjects.push_back(22);
        test.Definition.RewardObjects.push_back(23);
        test.Definition.RewardObjects.push_back(24);
        CThingObject::Plan.push_back(CThingObject::Spawn(21, 0));
        CThingObject::Plan.push_back(CThingObject::Spawn(22, &missingTC));
        CThingObject::Plan.push_back(CThingObject::Spawn(23, &nullTC));
        CThingObject::Plan.push_back(CThingObject::Spawn(24, &success));
    }
    if (!test.Complete() || !test.CheckFinish() || !test.CheckMessages(false) ||
        !CThingObject::Correct || test.Stats.Gold != 19 || test.Stats.RewardGold != 19 ||
        test.Stats.Renown != 5 || !test.Stats.CorrectGoldDisplay)
        return false;
    if (empty)
        return CThingObject::Calls == 0 && test.Weapons.SpaceCalls == 0 && item.Calls == 0;
    return CThingObject::Calls == 4 && test.Weapons.SpaceCalls == 1 && test.Weapons.LastDef == 24 &&
           test.Hero.LastInventoryType == TCI_INVENTORY_WEAPONS &&
           item.Calls == (full ? 0U : 1U) &&
           (full || (item.Correct && item.ReceivedInventory == &test.Weapons));
}

int main()
{
    if (!RejectedQuest() || !HeroUnavailable(true) || !HeroUnavailable(false))
        return 1;
    if (!MissingCard(false) || !MissingCard(true))
        return 2;
    if (!CheckQuestClass(true, false, true, true) || !CheckQuestClass(true, false, true, false) ||
        !CheckQuestClass(true, false, false, false) || !CheckQuestClass(false, true, true, false) ||
        !CheckQuestClass(false, false, true, false))
        return 3;
    if (!Rewards(false, false) || !Rewards(true, false) || !Rewards(false, true))
        return 4;
    puts("004b1d30_TEST PASS");
}