// CAIStateGroup_MinionReactToEnemy::Update() -- while the enemy is alive and still a target to attack: back off (to 2.4 units beyond the creature, away from the enemy) when the enemy is within 5 units in XY, then keep looking at the enemy; terminate when the enemy dies, stops being a target, or a move or look fails
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937DA0, length 450 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy), IsToAttackTarget(CThingAICreature*, CThingCreatureBase*)
// @ 0x00864D00 (__fastcall), CThing::PeekPos @ 0x004C73D0 (the XY distance helper), CThingCreatureBase::PeekPos
// @ 0x006A4D00, C3DVector::AccurateNormalise @ 0x00A14440, AIAction_MoveToPosProximity @ 0x0097E890, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C, by value),
// AIAction_LookAtThing @ 0x0097D6F0, Terminate @ 0x008FCF40.
// Data: 25.0f @ 0x0123003C, 2.4f @ 0x0129234C.
// Shape: the XY distance is an inline helper on CThing* (position references taken second-argument first, explicit dx/dy)
// stored to a float local before the `< 25` test (the x87 temporaries are popped first); C3DVector has a user copy
// constructor, so `creature position + away` is built straight into the argument slot; the back-off is `away *= 2.4f`
// in place after the out-of-line Normalise; the look target converts through a template copy constructor.
// Layout: Ego_r.pdb CAIStateGroup_MinionReactToEnemy (PEnemy +0x14, CIntelligentPointer<CThingCreatureBase>).
#include "engine/C3DVector.h"
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CThingAICreature : public CThingCreatureBase { public: };
// NF34 name fix: Ego_r.pdb + FableWin.pdb ?IsToAttackTarget@NCreatureAI@@YI_NPAVCThingAICreature@@PBVCThingCreatureBase@@@Z.
namespace NCreatureAI { bool __fastcall IsToAttackTarget(CThingAICreature* creature, const CThingCreatureBase* target); }   // 0x00864D00
inline float GetDistanceSquaredXY(const CThing* a, const CThing* b)
{
    const C3DVector& posB = b->GetPos();
    const C3DVector& posA = a->GetPos();
    float dx = posA.X - posB.X;
    float dy = posA.Y - posB.Y;
    return dx * dx + dy * dy;
}

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem((CBaseObject*)(other.GetPItem())); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(((CThing*)thing.GetPItem())->m_flags91 & 1); }

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h;
// AIAction_LookAtThing 0x0097D6F0 is the header's protected member (_AIACTIONS, Ego_r).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_MinionReactToEnemy : CAIStateGroupBase {
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }   // (the row's helper, moved here from its local base model)
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x14
    virtual void Update();
};

void CAIStateGroup_MinionReactToEnemy::Update()
{
    for (;;) {
        if (!IsThingAlive(PEnemy))
            break;
        if (!NCreatureAI::IsToAttackTarget(GetCreature(), PEnemy.GetPItem()))
            break;
        float distanceSquared = GetDistanceSquaredXY(GetCreature(), PEnemy.GetPItem());
        if (distanceSquared < 25.0f) {
            C3DVector away = GetCreature()->PeekPos() - PEnemy.GetPItem()->PeekPos();
            away.AccurateNormalise();
            away *= 2.4f;
            bool arrived;
            if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, GetCreature()->PeekPos() + away, 0.5f, arrived, AI_MOVEMENT_WALKING, true, false, true, 0,
                                            true, 0.0f) != 2)
                break;
        }
        if (!IsThingAlive(PEnemy))
            break;
        long result = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PEnemy, 0xb, 0.0027777f);
        if (result != 4 && result != 2)
            break;
    }
    Terminate();
}