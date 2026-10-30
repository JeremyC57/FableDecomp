// CQ_HeroSoulsNostroScript::Main() -- Hero Souls (Nostro): bind the five entity scripts, wait for GraveyardCircle, play the start cut-scene, give the quest card and start screen, set the objective, spawn the creature-generation and hit-watch threads, wait for Nostro's defeat, then play the success cut-scene, log the story entry and complete the quest
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D87CB0, 1440 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, operator new 0x00BFEA1A, AddEntityScriptBinding 0x00CB8230
//   (binding vtable 0x012D4E68), PostAddScriptedEntities 0x00CB8930, IsActiveThreadTerminating 0x00CB7940,
//   CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012D4D40), AddSpawnedFunction 0x00CB7E50, PlayCutScene 0x00D89AD0 (by-value
//   name; manifest label "Main_InitializeFourierAnalysis_2" is junk), AddLogbookStoryEntryString 0x00CBE960 (__fastcall);
//   allocators 0x00D88420 / 0x00D885E0 / 0x00D88EC0 / 0x00D892A0 / 0x00D890A0; threads DealWithCreatureGeneration
//   0x00D89710, WatchForNostroHit 0x00D88260; CGameScriptInterface slots 7, 12, 64, 288, 295, 296, 309, 374, 376, 655.
// Layout: CScriptBase 0x40, Game +0x40, PMasterData +0x44 (its bool +0x68 = Nostro defeated), QuestStartScreenShown +0x4D.
// Shape: binding model and WAIT_UNTIL loops of the landed CV_ChickenKickingScript::Main 0x00E62A00.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960
struct CNostroMasterData {
    unsigned char _pad[0x68];
    bool NostroDefeated;                                 // +0x68
};
namespace NScript {
class CQ_HeroSoulsNostroScript : public CScriptBase {
public:
    struct CSummonerToKill { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };          // 0x00D88420
    struct CGuardNearGraveyardPath { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };  // 0x00D885E0
    struct CUndeadSoldier { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };           // 0x00D88EC0
    struct CNostro { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };                  // 0x00D892A0
    struct CSummonedCreature { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& thing); };        // 0x00D890A0
    CGameScriptInterface* Game;                          // +0x40
    CNostroMasterData*    PMasterData;                   // +0x44
    unsigned char         _pad_0x48[5];
    bool                  QuestStartScreenShown;         // +0x4D
    void Main();
    void PlayCutScene(CCharString name);                 // 0x00D89AD0
    void DealWithCreatureGeneration();                   // 0x00D89710
    void WatchForNostroHit();                            // 0x00D88260
};
} // namespace NScript
using NScript::CQ_HeroSoulsNostroScript;

void NScript::CQ_HeroSoulsNostroScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsNostroScript>("SummonerToKill", this, &CSummonerToKill::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsNostroScript>("GuardNearGraveyardPath", this, &CGuardNearGraveyardPath::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsNostroScript>("UndeadSoldier", this, &CUndeadSoldier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsNostroScript>("Nostro", this, &CNostro::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsNostroScript>("SUMMONED_CREATURE", this, &CSummonedCreature::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("GraveyardCircle")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PlayCutScene("CS_HEROSOULS_SOUL3_NOSTRO_START");
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_NOSTRO", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    QuestStartScreenShown = true;
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_13", "GraveyardCircle", "Graveyard");
    AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsNostroScript>("DealWithCreatureGeneration", 0, this, &NScript::CQ_HeroSoulsNostroScript::DealWithCreatureGeneration), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsNostroScript>("WatchForNostroHit", 0, this, &NScript::CQ_HeroSoulsNostroScript::WatchForNostroHit), CCharString(""));
    Game->FadeScreenIn();
    while (!PMasterData->NostroDefeated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    PlayCutScene("CS_HEROSOULS_SOUL3_NOSTRO_SUCCESS");
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_370");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}