// NScript::CQ_TraderEscortScript::CDarkwoodTrader::~CDarkwoodTrader() -- implicit entity-script destructor of the Darkwood escort trader: deregister RegulateBanterComment, RegulateFollowStateComment and IncubationTime (the CTimer dtors, reverse order), then ~CEntityScriptBase
// Leaf: ??1CDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAE@XZ
// Naming: the global 0x0143E8F8 is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E01710, 65 bytes, thiscall, tail `jmp ~CEntityScriptBase` 0x00F35B40.
// Identity (proven, two sources): its only caller is the scalar deleting dtor 0x00E016F0, slot 0 of the RTTI-less entity vtable
//   0x012DF424, which the landed ctor 0x00E01670 stores (??0CDarkwoodTrader@CQ_TraderEscortScript@NScript@@...) and whose slot 2 is the
//   landed ?Init@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXXZ 0x00E04BD0; Ego_r.pdb / FableWin.pdb
//   ??1CDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAE@XZ (Ego_r 0x00E43DF0, between ??0 0x00E43D30 and ??_G 0x00E43E80). The three
//   DeregisterTimer calls are on +0x3C / +0x38 / +0x2C = Ego_r CDarkwoodTrader's CTimer members RegulateBanterComment /
//   RegulateFollowStateComment / IncubationTime. It stores no vtable: the compiler-generated destructor. The catalog's bare
//   `CDarkwoodTrader` class is the nested NScript::CQ_TraderEscortScript::CDarkwoodTrader.
// Callees: CTimer::~CTimer inline (engine/CTimer.h: pDSTGame 0x0143E8F8 interface slot 88 (+0x160) DeregisterTimer(TimerIndex)),
//   ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (tail).
// Layout: Ego_r.pdb CQ_TraderEscortScript::CDarkwoodTrader (0x40, no STL member: retail's, as the landed ctor confirms): CEntityScriptBase
//   0x14, ParentClass +0x14, PMasterData +0x18, Infected +0x1C, BrainState +0x20, CurrentAIState +0x24, PreviousAIState +0x28,
//   IncubationTime +0x2C, TraderHealthID +0x30, GreetedBuddy / LeadToCamp / InSafeZone +0x34..+0x36, RegulateFollowStateComment +0x38,
//   RegulateBanterComment +0x3C. engine/CEntityScriptBase.h, engine/CLevelScriptBase.h, engine/CTimer.h (FABLE_RETAIL_NSCRIPT).
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E01670), which emits the vtable, ??_G and this implicit dtor (as the
//   landed CBriarRose dtor 0x00D9FED0).
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CDarkwoodTrader : public CEntityScriptBase {
    public:
        CQ_TraderEscortScript*  ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                            // +0x18
        long                    Infected;                               // +0x1c
        long                    BrainState;                             // +0x20
        long                    CurrentAIState;                         // +0x24
        long                    PreviousAIState;                        // +0x28
        CTimer                  IncubationTime;                         // +0x2c
        long                    TraderHealthID;                         // +0x30
        bool                    GreetedBuddy;                           // +0x34
        bool                    LeadToCamp;                             // +0x35
        bool                    InSafeZone;                             // +0x36
        CTimer                  RegulateFollowStateComment;             // +0x38
        CTimer                  RegulateBanterComment;                  // +0x3c
        CDarkwoodTrader(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
        virtual void Main();
        virtual void Init();
        virtual void OnPersist(CPersistContext& context);
        virtual void OnPredicateFail();
    public:
        virtual CScriptBase* GetParentScript() const;
    };
};
FABLE_STATIC_ASSERT(sizeof(CQ_TraderEscortScript::CDarkwoodTrader) == 0x40);
CQ_TraderEscortScript::CDarkwoodTrader::CDarkwoodTrader(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me)   // 0x00E01670
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}