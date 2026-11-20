// CAIStateGroup_BanditKingCharge::Update() -- with a live target: run at it (MoveToThingProximity, 4m), and on arrival with the target still alive perform the "BK_CHARGE_ATTACK" attack action; terminate
// Leaf: ?Update@CAIStateGroup_BanditKingCharge@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961790, length 239 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/0x00A01B50/0x00A01B90, CAIStateGroupBase::
// AIAction_MoveToThingProximity @ 0x0097E380 (CIntelligentPointer<const CThing> by value, vtable 0x01237F1C),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_BanditKingAttack
// ctor @ 0x008C9410 (creature, target, const CCharString& anim, bool, bool), CAIStateGroupBase::
// AIAction_CombatPerformActionAndWait @ 0x0097DA40, CCombatActionBase::~CCombatActionBase @ 0x0062E900,
// CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the intelligent pointer argument is built in the argument area through an inline converting copy ctor
// from PTargetThing (base ctor, vtable store, then GetPItem -- a T* ctor evaluates GetPItem first); a one-byte option
// temporary is passed by reference; the animation name is a named local built before the creature is read.
// NF175 (2026-11-20, q4967): AIAction_MoveToThingProximity 0x0097E380 respelled in place from the derived stand-in
//   ?AIAction_MoveToThingProximity@CAIStateGroup_BanditKingCharge@@QAEJJV?$CIntelligentPointer@$$CBVCThing@@@@MABUCMoveOptions@@J_N2222@Z to the
//   PDB ?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z
//   (Ego_r.pdb 0x008491B0, protected), declared on the row-local CAIStateGroupBase: the CMoveOptions stand-in is the PDB `bool&` (an
//   uninitialised local, as the landed 0x0094F000), the last argument the PDB float, the enums engine/EAIAction.h / EMovementType.h.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

class CBaseObject;
struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject*a0);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    virtual void v00();
    FABLE_FORCEINLINE CIntelligentPointer(T* item) { SetPItem((CBaseObject*)((CThing*)item)); }
    FABLE_FORCEINLINE CIntelligentPointer(const CIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
    FABLE_FORCEINLINE CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }   // converting copy
};

FABLE_FORCEINLINE bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0)
    {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

class CCreatureActionBase
{ public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

class CCombatActionBase : public CCreatureActionBase
{ public:
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    virtual ~CCombatActionBase();                           // 0x0062E900
};

class CCreatureAction_BanditKingAttack : public CCombatActionBase
{ public:
    unsigned char _own[0x04];
    CCreatureAction_BanditKingAttack(CThing* creature, CThing* target, const CCharString& anim, bool a, bool b);   // 0x008C9410
};

class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

class CAIStateGroupBase
{
public:
    virtual void v00();
public:
    void Terminate();   // 0x008FCF40
protected:
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target, float proximity,
                                                  bool& pArrived, EMovementType movement, bool a, bool b, bool c, bool d,
                                                  float e);     // 0x0097E380
};

struct CAIStateGroup_BanditKingCharge
: public CAIStateGroupBase {
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
    void Update();
};

void CAIStateGroup_BanditKingCharge::Update()
{
    bool arrived;
    if (IS_THING_ALIVE(PTargetThing)
        && AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, CIntelligentPointer<const CThing>(PTargetThing), 4.0f,
                                         arrived, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f) == AI_ACTION_FINISHED
        && IS_THING_ALIVE(PTargetThing))
    {
        CCharString anim("BK_CHARGE_ATTACK", -1);
        CThing* creature = PBrain->Creature;
        AIAction_CombatPerformActionAndWait(
            &CCreatureAction_BanditKingAttack(creature, ((CThing*)PTargetThing.GetPItem()), anim, true, false), false);
    }
    Terminate();
}