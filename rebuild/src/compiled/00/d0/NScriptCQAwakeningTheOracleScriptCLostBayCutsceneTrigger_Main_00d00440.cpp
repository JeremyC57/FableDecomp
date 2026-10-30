// NScript::CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Main() -- the Lost Bay cutscene trigger: unless the cutscene has played, make Me persistent, age the hero a year, play CS_LOSTBAY_INTRO, mark it played, pause 10 s and start Briar's Lost Bay message conversation, then wait on the conversation; finally remove Me
// Leaf: ?Main@CLostBayCutsceneTrigger@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D00440, 327 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 1 (Main) of the entity-script vtable 0x012C7C5C, which the landed
//   NScript::CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Alloc 0x00CFC7D0 stores; Ego_r.pdb
//   ?Main@CLostBayCutsceneTrigger@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ (0x00D01390). Catalog label sub_00d00440.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (Ego_r `QBE`), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CQ_AwakeningTheOracleScript::PlayCutscene(CCharString, bool) 0x00CFF9A0 (Ego_r
//   ?PlayCutscene@CQ_AwakeningTheOracleScript@NScript@@QAEXVCCharString@@_N@Z); CGameScriptInterface slots 7 NewScriptFrame,
//   70 GetHero, 108 RemoveThing, 170 SetHeroAge, 171 GetHeroAge, 364 AddNewConversation, 366 AddLineToConversation,
//   368 IsConversationActive, 376 Pause, 477 SetThingPersistent.
// Literals: "CS_LOSTBAY_INTRO" 0x012C82EC, "TEXT_QST_B03_BRIAR_LOSTBAY_MESSAGE" 0x012C82C8, 1.0f 0x0122DED8, 10.0f immediate.
// Layout: Ego_r CLostBayCutsceneTrigger (0x1c = the Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14, PMasterData +0x18. CQ_AwakeningTheOracleScript at Ego_r - 0x14: PlayedLostBayCS +0x4a.
// Shape: the by-value CCharString argument is built in its stack slot (user copy constructor declared); the conversation
//   handle lives in edi; the played test skips straight to the RemoveThing tail.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30 (declared: forces the in-slot build)
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool PlayedOpeningCS;                               // +0x49
    bool PlayedLostBayCS;                               // +0x4a
    bool PlayedNecroCS;                                 // +0x4b
    void PlayCutscene(CCharString name, bool wait);     // 0x00CFF9A0
    class CLostBayCutsceneTrigger : public CEntityScriptBase {
    public:
        CQ_AwakeningTheOracleScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    private:
        virtual void Main();                            // slot 1
    };
};

void CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Main()
{
    if (!ParentClass->PlayedLostBayCS) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingPersistent(Me, true);
        Game->SetHeroAge(Game->GetHeroAge() + 1.0f);
        ParentClass->PlayCutscene("CS_LOSTBAY_INTRO", true);
        ParentClass->PlayedLostBayCS = true;
        Game->Pause(10.0f);
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B03_BRIAR_LOSTBAY_MESSAGE", false, Game->GetHero(), Game->GetHero());
        while (!Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->RemoveThing(Me, false, true);
}
}   // namespace NScript