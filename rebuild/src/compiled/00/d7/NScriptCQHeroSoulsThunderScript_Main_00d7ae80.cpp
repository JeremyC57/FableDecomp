// CQ_HeroSoulsThunderScript::Main() -- Hero Souls (Thunder): bind HeroThunder, wait for Knothole Glade; on first run open the KG gate, pick the start line, play the start cut-scene, switch the arena quests/sound themes/objective and fade in; have Jack speak once, wait for Thunder's defeat, then play the success cut-scene, give morality, log the story and complete the quest
// Leaf: ?Main@CQ_HeroSoulsThunderScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7AE80, 1392 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0, operator new 0x00BFEA1A,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012D364C, allocator 0x00D7B400), PostAddScriptedEntities 0x00CB8930,
//   IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840, map<CCharString,CCharString>::operator[] 0x009AC700,
//   PlayCutscene 0x00D7C470 (by-value name), AddLogbookStoryEntryString 0x00CBE960 (__fastcall), pGlobalScriptDef 0x0143E90C;
//   CGameScriptInterface slots 7, 12, 64, 70, 72, 156, 276, 280, 288, 296, 364, 366, 374, 425, 655, 665.
// Layout: Game +0x40, PMasterData +0x44 (+0x63 Thunder defeated, +0x70 arena state, +0x74 / +0xD7 line selectors),
//   StartDone +0x48, SkipStart +0x49, JackSpoke +0x4A, CutSceneArgs map +0x4C.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CScriptThing { public:                                   // 12 bytes {vptr, PImp.Data, PImp.Info}
    void* VTable;
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
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
#include "engine/CCountedPointer.h"

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
class CScriptDef { public:
    unsigned char _pad[0xc9c];
    float ThunderMoralityReward;                         // +0xc9c
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CStringMap {                                      // std::map<CCharString, CCharString> (12 bytes)
    void* _impl[3];
    CCharString& operator[](const CCharString& key);     // 0x009AC700
};
struct CThunderMasterData {
    unsigned char _pad[0x63];
    bool ThunderDefeated;                                // +0x63
    unsigned char _pad_0x64[0x70 - 0x64];
    long ArenaState;                                     // +0x70
    bool LineA;                                          // +0x74
    unsigned char _pad_0x75[0xd7 - 0x75];
    bool LineB;                                          // +0xD7
};
namespace NScript {
class CQ_HeroSoulsThunderScript : public CScriptBase {
public:
    struct CHeroThunder { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsThunderScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D7B400
    CGameScriptInterface* Game;                          // +0x40
    CThunderMasterData*   PMasterData;                   // +0x44
    bool                  StartDone;                     // +0x48
    bool                  SkipStart;                     // +0x49
    bool                  JackSpoke;                     // +0x4A
    unsigned char         _pad_0x4b;
    CStringMap            CutSceneArgs;                  // +0x4C
    void Main();
    void PlayCutscene(CCharString name);                 // 0x00D7C470
};
} // namespace NScript
using NScript::CQ_HeroSoulsThunderScript;

void NScript::CQ_HeroSoulsThunderScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsThunderScript>("HeroThunder", this, &CHeroThunder::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!StartDone && !SkipStart) {
        if (IsActiveThreadTerminating())
            return;
        Game->OpenDoor(Game->GetThingWithScriptName("KG_Gate"));
        if (PMasterData->LineA) {
            if (IsActiveThreadTerminating())
                return;
            CutSceneArgs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_A";
        } else if (PMasterData->LineB) {
            if (IsActiveThreadTerminating())
                return;
            CutSceneArgs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            CutSceneArgs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_C";
        }
        PlayCutscene("CS_HEROSOULS_SOUL1_THUNDER_START");
        StartDone = true;
        Game->ActivateQuest("Q_HeroSoulsArena");
        Game->ActivateQuest("Q_HeroSoulsPermanentArena");
        Game->DeactivateQuest("Q_ArenaHoldingScript", 0);
        Game->SetSoundThemesAsEnabledForRegion("ArenaExterior", false);
        Game->SetSoundThemesAsEnabledForRegion("ArenaCells", false);
        Game->SetSoundThemesAsEnabledForRegion("Arena", false);
        PMasterData->ArenaState = 2;
        Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_03", "Arena", "KnotholeGlade");
        Game->FadeScreenIn();
    }
    if (!JackSpoke) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_THUNDER_10", false, Game->GetHero(), Game->GetHero());
        if (PMasterData->LineA) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_THUNDER_20", false, Game->GetHero(), Game->GetHero());
        }
        JackSpoke = true;
    }
    while (!PMasterData->ThunderDefeated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PlayCutscene("CS_HEROSOULS_SOUL1_THUNDER_SUCCESS");
    Game->GiveHeroMorality(pGlobalScriptDef->ThunderMoralityReward);
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_355");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
}