// CInputProcessParalysed::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- count held presses of game action 9; past ten, a live hero breaks off the drain holding it (current action = CCombatAction_GenericBreakOffDrain)
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690D30, length 116 bytes. Vtable 0x01237D88 slot 4.
// Callees: CInputProcessGameBase::IsEventGameAction @ 0x00687CF0, IsEventButtonHeld @ 0x00687BD0, CPlayer::GetHeroCreature
// @ 0x00487DC0 (MyPlayer +0x18), CCombatAction_GenericBreakOffDrain::CCombatAction_GenericBreakOffDrain(CThingCreatureBase*)
// @ 0x00690DB0 (stores vtable 0x0125BF6C; manifest label "CCombatAction_GenericResponseToDecapitation" is propagated),
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, CCombatActionBase::~CCombatActionBase @ 0x0062E900.
// Model: retail CCombatActionBase is 0x114 bytes (the frame), as the landed CCreatureAction_JackFall::FinishAction
// 0x008C2490; the action is a temporary built inside the call (`&CAction(creature)`) and destroyed through the base
// destructor. The press counter sits at +0x28, just past the CInputProcessGameBase members (engine/CInputProcessParalysed.h
// flattens the base; the counter is modelled as m_pressCount). The hero's dead flag is bit 0 of +0x91.
class CInputEvent;
class CProcessedInput;
class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];
    virtual ~CCombatActionBase();                          // 0x0062E900
};
class CCombatAction_GenericBreakOffDrain : public CCombatActionBase { public:
    CCombatAction_GenericBreakOffDrain(CThingCreatureBase* creature);   // 0x00690DB0
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
class CPlayer {
public:
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessParalysed : public CInputProcessGameBase {
public:
    long m_pressCount;                             // +0x28
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessParalysed::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventGameAction(9, event) && IsEventButtonHeld(event)) {
        if (++m_pressCount > 10) {
            CThingCreatureBase* hero = MyPlayer->GetHeroCreature();
            if (hero && !hero->m_dead)
                hero->SetCurrentAction(&CCombatAction_GenericBreakOffDrain(hero));
        }
    }
}