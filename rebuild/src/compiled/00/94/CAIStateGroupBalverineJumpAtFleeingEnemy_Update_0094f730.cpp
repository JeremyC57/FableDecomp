// CAIStateGroup_BalverineJumpAtFleeingEnemy::Update() -- schedule next attempt, lunge at a live target, then break off after successful contact.
// Retail0094F730,230 bytes. State/brain use shared headers; thing Dead is retail+91 vs donor+a1.
// Actions have the verified0x114-byte retail combat base, rather than donor0x12c.
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"
#include "engine/CAIBrain.h"
class CThing;
class CThingCreatureBase;
struct CRetailThing { unsigned char _prefix[0x91]; unsigned char Dead:1; };
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable; CThing* Object;
    CBaseObject* GetPItem() const; // 00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) {
    if(((CThing*)p.GetPItem())!=0) {if(!((CRetailThing*)p.GetPItem())->Dead) return true;}
    return false;
}
class CCombatActionBase { public:
    virtual void v00(); unsigned char _body[0x110];
    virtual ~CCombatActionBase(); // 0062E900
};
class CCreatureAction_BalverineLungeAttack : public CCombatActionBase { public:
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase*,CThing*); // 008CCC70
};
class CCreatureAction_BalverineBreakOffFromCombatLong : public CCombatActionBase { public:
    CCreatureAction_BalverineBreakOffFromCombatLong(CThingCreatureBase*,CThing*); // 008CCD30
};
extern long g_framesPerSecond; // 01375550
class CWorld { public: static long __fastcall GetFrame(); }; // 0049D870
struct CJumpAtFleeingMethods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    CAIBrain* Brain() const {return *(CAIBrain* const*)_pad_0x04;}
    const CBaseIntelligentPointer& Target() const {return *(const CBaseIntelligentPointer*)PTarget;}
    long PerformActionAndWait(const CCombatActionBase*,bool); // 0097DA40
    void Terminate(); // 008FCF40
    void Update();
};
void CJumpAtFleeingMethods::Update() {
    long fps=g_framesPerSecond;
    WaitUntilWorldFrameForNextAttempt=(long)(fps*5.0f+CWorld::GetFrame());
    if(IsAlive(Target())) {
        CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
        long result=PerformActionAndWait(&CCreatureAction_BalverineLungeAttack(creature,((CThing*)Target().GetPItem())),false);
        if(result==2 && IsAlive(Target())) {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            long result=PerformActionAndWait(&CCreatureAction_BalverineBreakOffFromCombatLong(creature,((CThing*)Target().GetPItem())),false);
            if(result==2) {Terminate();return;}
        }
    }
    Terminate();
}