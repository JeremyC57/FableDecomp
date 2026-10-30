// CQ_HeroSoulsMotherScript::Main() -- the Hero Souls (mother) quest's main: bind the SummonerToKill / HS_Screamer entity scripts, wait for OakBay or the Oakvale memorial garden; in Oak Bay have Jack talk to the hero until he reaches the MotherJackChatMarker, then wait for the garden, play the mother start cutscene once, hand over the quest card, kick off the start screen, set the objective, spawn CreateScreamers, fade in, wait for the soul, and unless Briar is dead play the success cutscene, log the story entry, set the next objective, complete the quest and fade in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D835D0, length 1808 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, CScriptBase::IsActiveThreadTerminating 0x00CB7940 /
//   AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50,
//   CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable 0x012D4604, CreateScreamers 0x00D84630, landed),
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), AddLogbookStoryEntryString 0x00CBE960 (__fastcall),
//   CQ_HeroSoulsMotherScript::PlayCutscene(CCharString) 0x00D848D0 (manifest label "Main_InitializeFourierAnalysis_1" is
//   propagated); binding allocators 0x00D83CF0 (SummonerToKill) / 0x00D83E60 (HS_Screamer) (binding vtable 0x012D4758);
//   the game script interface (+0x40) slots 7, 12, 64, 70, 72, 288, 295, 296, 309, 364, 366, 368, 374, 376, 655.
// Layout (retail = Ego_r.pdb - 0x14, see the landed ctor 0x00D83400): PlayedMotherCS +0x48, QuestStartScreened +0x64;
//   the master data (+0x44) CQ_SunnyvaleMasterData: KilledBriar +0x65, CollectedSoulFromMother +0x66.
// Shape: bindings / spawned function as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20 (forceinline
//   CSpawnedFunc ctor and SpawnFunction); the re-open-conversation arm is `if (!IsActiveThreadTerminating()) conversation = ...;
//   else return;` -- the plain `if (Term) return;` form lays the shared ~CScriptThing exit inline there instead of after the spawn.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000();
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall AddLogbookStoryEntryString(const CCharString& tag);   // 0x00CBE960
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptDataBase;
class CEntityScriptBase;
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
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x65];
    bool KilledBriar;                                   // +0x65
    bool CollectedSoulFromMother;                       // +0x66
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HeroSoulsMotherScript : public CScriptBase {
public:
    struct CSummonerToKill { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsMotherScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D83CF0
    struct CHSScreamer { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsMotherScript& parent, CScriptDataBase* data, const CScriptThing& thing); };       // 0x00D83E60
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool                    PlayedMotherCS;             // +0x48
    unsigned char           _pad49[0x64 - 0x49];        // csargs map +0x4c, ScreamersAlive +0x58, BarToFill +0x5c, BarID +0x60
    bool                    QuestStartScreened;         // +0x64
    bool                    GotSuperScreamer;           // +0x65
    void Main();
    void CreateScreamers();                             // 0x00D84630
    void PlayCutscene(CCharString name);                // 0x00D848D0
};
void CQ_HeroSoulsMotherScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsMotherScript>("SummonerToKill", this, &CSummonerToKill::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsMotherScript>("HS_Screamer", this, &CHSScreamer::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!(Game->IsRegionLoaded("OakBay") || Game->IsRegionLoaded("OakvaleMemorialGarden"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->IsRegionLoaded("OakBay")) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Game->GetHero(), true, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_MOTHER_10", false, Game->GetHero(), Game->GetHero());
        CScriptThing marker = Game->GetThingWithScriptName("MotherJackChatMarker");
        while (!IsDistanceBetweenThingsUnder(marker, Game->GetHero(), 50.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsConversationActive(conversation)) {
            if (!IsActiveThreadTerminating())
                conversation = Game->AddNewConversation(Game->GetHero(), true, false);
            else
                return;
        }
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_MOTHER_20", false, Game->GetHero(), Game->GetHero());
    }
    while (!Game->IsRegionLoaded("OakvaleMemorialGarden")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!PlayedMotherCS) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_HEROSOULS_SOUL2_MOTHER_START");
        PlayedMotherCS = true;
    }
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_MOTHER", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    QuestStartScreened = true;
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_09", "OakvaleMemorialGarden", "OakBay");
    AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsMotherScript>("CreateScreamers", 0, this, &CQ_HeroSoulsMotherScript::CreateScreamers), CCharString(""));
    Game->FadeScreenIn();
    while (!PMasterData->CollectedSoulFromMother) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.8f);
    if (PMasterData->KilledBriar) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    PlayCutscene("CS_HEROSOULS_SOUL2_MOTHER_SUCCESS");
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_360");
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_10", "NorthernWastes3", "NorthernWastes3");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}
}