// CGameScriptInterface::KickOffQuestStartScreen(const CCharString& questName, bool showScreen, bool autoSave) const -- either just save the quest card's initial values (then, when asked and quest screens are enabled, record the activated quest and autosave), or activate the quest-start screen and, while the hero is player-controlled, yield frames until it closes; then give the hero the quest tutorial (category 0x1c)
// Grade: DIFFER(437v445) -- block layout only: retail places the shared epilogue after the quest-card path and gives the tutorial branch its own copy; VC7.1 shares one epilogue and lays the tutorial first
// Next: real CTutorialManager/GiveHeroTutorial inline body from a header (a same-TU body is not auto-inlined); otherwise lane gap: tail duplication.
// Leaf: ?KickOffQuestStartScreen@CGameScriptInterface@@UBEXABVCCharString@@_N1@Z
class CCharString { public:
    void* PStringData;
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTCPointer()
    {
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
        return (T*)((it == map->last || T::INTERFACE_TYPE < it->first) ? map->last : it)->second;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        *out = (T*)((it == map->last || T::INTERFACE_TYPE < it->first) ? map->last : it)->second;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
#include "engine/CPlayerManager.h"
enum ETutorialCategory { TUTORIAL_CATEGORY_QUEST_START = 0x1c };
class CTCHero { public:
    enum { INTERFACE_TYPE = 0x29 };
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CTCQuestCompletionUI { public:
    enum { INTERFACE_TYPE = 0xfc };
    enum CompletionType { QUEST_START = 0 };
    unsigned char _pad00[0xc];
    bool          Active;                               // +0x0c
    unsigned char _pad0d[0x1c - 0xd];
    bool          Closing;                              // +0x1c
    unsigned char _pad1d[0xc0 - 0x1d];
    bool          Skipped;                              // +0xc0
    void Activate(CompletionType type, const CCharString& questName, bool b1, const CWideString& summary, bool b2);   // 0x005E4740
    void SaveInitialValues(const CCharString& questName);   // 0x005E4860
};
class CWorld { public:
    bool AutoSaveQuestStart();                          // 0x004A0CD0
};
class CQuestManager { public:
    void AddActivatedQuestName(const CCharString& name);   // 0x004B2FE0
};
class CSuspendableProcessMicrothread { public:
    unsigned char _pad00[5];
    bool          Stop;                                 // +0x05
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
extern bool g_bQuestFinishScreensEnabled;               // 0x01375741
extern CSuspendableProcessMicrothread* g_pScriptRunner; // 0x013D2838

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::KickOffQuestStartScreen(const CCharString& questName, bool showScreen, bool autoSave) const
{
    CTCQuestCompletionUI* ui;
    if (!showScreen) {
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (hero->HasTC(CTCQuestCompletionUI::INTERFACE_TYPE))
            ui = hero->GetTCPointer<CTCQuestCompletionUI>();
        ui->SaveInitialValues(questName);
        if (autoSave && g_bQuestFinishScreensEnabled) {
            g_pQuestManager->AddActivatedQuestName(questName);
            World->AutoSaveQuestStart();
        }
        return;
    }
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero->HasTC(CTCQuestCompletionUI::INTERFACE_TYPE))
        ui = hero->GetTCPointer<CTCQuestCompletionUI>();
    {
        CWideString summary(L"");
        ui->Activate(CTCQuestCompletionUI::QUEST_START, questName, true, summary, true);
    }
    if (IsHeroControlledByPlayer()) {
        while (ui->Active && !ui->Closing && !ui->Skipped && !g_pScriptRunner->Stop)
            NewScriptFrame();
        if (g_pScriptRunner->Stop)
            return;
    }
    hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHero* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc) && tc)
        tc->GiveTutorialForCategory(TUTORIAL_CATEGORY_QUEST_START, false);
}
