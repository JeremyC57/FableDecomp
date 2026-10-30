// CQ_HeroSoulsBriarScript::Main() -- the Hero Souls (Briar Rose) quest's main: bind the BriarRose / BRIAR_ROSE_IMPOSTER / HSB_MagicBarrier entity scripts; on the first run wait for the intro cutscene, set the objective, activate Q_HeroSoulsMother, auto-save and fade in; wait while the Northern Wastes are loaded without the start screen; give Jack's second-soul line, set the objective, wait for the start screen, then fight Briar Rose (Jack taunts at boss phases 1 and 2), wait for her death, play the soul-3 intro, close the magic barrier quest, count the delivered soul, log, give the good-soul morality, set the next objective, complete the quest and fade in
// Leaf: ?Main@CQ_HeroSoulsBriarScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80E40, length 2016 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, CScriptBase::IsActiveThreadTerminating 0x00CB7940 /
//   AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930, AddLogbookStoryEntryString 0x00CBE960
//   (__fastcall), CQ_HeroSoulsBriarScript::PlayCutscene(CCharString) 0x00D823A0 (landed); binding allocators 0x00D81630
//   (BriarRose) / 0x00D81750 (BRIAR_ROSE_IMPOSTER) / 0x00D81B60 (HSB_MagicBarrier) (binding vtable 0x012D42E4);
//   pGlobalScriptDef 0x0143E90C; the game script interface (+0x40) slots 7, 12, 64, 70, 72, 156, 264, 276, 280, 288, 296,
//   364, 366, 374, 376, 522, 565, 655, 704.
// Layout (retail = Ego_r.pdb - 0x14, see the landed ctor 0x00D80C30): PlayedBriarCS +0x48, KickOffQuestStartScreen +0x70;
//   master data (+0x44) CQ_SunnyvaleMasterData: KilledBriar +0x65, DeliveredSoul +0x6c; CScriptDef (retail = PDB - 0xb0):
//   HS_GoodSoulTaken +0xc9c.
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
    unsigned char _pad66[0x6c - 0x66];
    long DeliveredSoul;                                 // +0x6c
};
class CScriptDef { public:
    unsigned char _pad0[0xc9c];
    float HS_GoodSoulTaken;                             // +0xc9c
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
namespace NScript {
class CQ_HeroSoulsBriarScript : public CScriptBase {
public:
    struct CBriarRose { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsBriarScript& parent, CScriptDataBase* data, const CScriptThing& thing); };          // 0x00D81630
    struct CBriarRoseImposter { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsBriarScript& parent, CScriptDataBase* data, const CScriptThing& thing); };  // 0x00D81750
    struct CHSBMagicBarrier { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsBriarScript& parent, CScriptDataBase* data, const CScriptThing& thing); };    // 0x00D81B60
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool                    PlayedBriarCS;              // +0x48
    unsigned char           _pad49[0x70 - 0x49];        // seh_BriarRose +0x4c, csargs +0x5c, two timers +0x68 / +0x6c
    bool                    KickOffQuestStartScreen;    // +0x70
    void Main();
    void PlayCutscene(CCharString name);                // 0x00D823A0
};
void CQ_HeroSoulsBriarScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsBriarScript>("BriarRose", this, &CBriarRose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsBriarScript>("BRIAR_ROSE_IMPOSTER", this, &CBriarRoseImposter::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsBriarScript>("HSB_MagicBarrier", this, &CHSBMagicBarrier::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!PlayedBriarCS) {
        if (IsActiveThreadTerminating())
            return;
        while (!PlayedBriarCS) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07", "OakvaleMemorialGarden", "OakBay");
        Game->ActivateQuest("Q_HeroSoulsMother");
        Game->AutoSaveQuestStart();
        Game->FadeScreenIn();
    }
    while (Game->IsRegionLoaded("NorthernWastes3") && !KickOffQuestStartScreen) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!KickOffQuestStartScreen) {
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(1.0f);
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_SECOND_SOUL", false, Game->GetHero(), Game->GetHero());
    }
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07", "OakvaleMemorialGarden", "OakBay");
    while (!KickOffQuestStartScreen) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTeleportingAsActive(false);
    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
    while (Game->EntityGetBossPhase(briar) != 1 && !(Game->GetHealth(briar) < 50.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(4.0f);
    long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_BRIAR_TAUNT_10", false, Game->GetHero(), Game->GetHero());
    while (Game->EntityGetBossPhase(briar) != 2 && !(Game->GetHealth(briar) < 50.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    conversation = Game->AddNewConversation(Game->GetHero(), false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_BRIAR_TAUNT_20", false, Game->GetHero(), Game->GetHero());
    while (!PMasterData->KilledBriar) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTeleportingAsActive(true);
    if (!PMasterData->KilledBriar)
        return;
    if (IsActiveThreadTerminating())
        return;
    PlayCutscene("CS_HEROSOULS_SOUL3_INTRO_JACK");
    Game->DeactivateQuest("Q_HeroSoulsBriarMagicBarrier", 0);
    ++PMasterData->DeliveredSoul;
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_365");
    Game->GiveHeroMorality(pGlobalScriptDef->HS_GoodSoulTaken);
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_11", "HeroGuildComplexInside", "HeroGuildComplexInside");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    Game->FadeScreenIn();
}
}