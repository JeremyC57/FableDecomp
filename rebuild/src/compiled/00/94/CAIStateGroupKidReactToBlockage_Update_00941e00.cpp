// CAIStateGroup_KidReactToBlockage::Update() -- look at the blocking creature, then (while it is still alive) throw a three-part tantrum (into, loop, out of), stopping at the first animation that does not complete; always terminate
// Leaf: ?Update@CAIStateGroup_KidReactToBlockage@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941E00, length 448 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThingCreature +0x2c), CThing::GetPosition @ 0x004C73D0,
// CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat, C3DVector, long, float) @ 0x0097D580 (repeat 1, flags 1, turn
// speed 1/72), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ST_TANTRUM_INTO" @ 0x012928A4,
// "ST_TANTRUM_LOOP" @ 0x01292894, "ST_TANTRUM_OUTOF" @ 0x01292880), CCreatureAction_PlayAnimation ctor @ 0x008424F0,
// AIAction_PerformActionAndWait @ 0x0097DB70, the action base dtor @ 0x00693EF0, Terminate @ 0x008FCF40.
// Shape: the creature is read through an inline brain getter (as the landed ChickenPanic 0x00911D90): PBrain is loaded before the
// argument pushes and the sound-pair zeros land before its address is pushed; a trailing `if (failed) { Terminate(); return; }`
// keeps the third result's setne and the dead `mov al` reload (both arms tail-merge into the one Terminate call).
// Layout: the state group is local (the engine header flattens the base); PThingCreature +0x2c (CIntelligentPointer<CThingCreatureBase>);
// CCreatureAction_PlayAnimation from the landed 0x008424F0 model (sizeof 0xb4).
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
enum EAIActionRepeat;                         // MS forward enum
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"

class CThingCreatureBase;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
    ~CCreatureAction_PlayAnimation();           // 0x00693EF0 (the base dtor)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    CThingCreatureBase* GetPCreature() const { return PCreature; }
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float turnSpeed);   protected: // 0x0097D580
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:        // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_KidReactToBlockage : CAIStateGroupBase {
    unsigned char _pad14[0x2c - 0x14];
    CIntelligentPointer<CThing> PThingCreature;     // +0x2c
    void Update();
};

#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())

void CAIStateGroup_KidReactToBlockage::Update()
{
    if (IS_THING_ALIVE(PThingCreature.Get())) {
        CThing* thing = PThingCreature.Get();
        if (AIAction_LookAtPos((EAIActionRepeat)(1), thing->GetPos(), 1, 0.013888889f) == 1) {
            Terminate();
            return;
        }
    }
    if (IS_THING_ALIVE(PThingCreature.Get())) {
        bool failed;
        {
            CCharString anim("ST_TANTRUM_INTO");
            failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), anim, CSoundPair(),
                                                                                 false, true, false, 0, 0), false) != 2;
        }
        if (!failed) {
            {
                CCharString anim("ST_TANTRUM_LOOP");
                failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), anim, CSoundPair(),
                                                                                     false, true, false, 0, 0), false) != 2;
            }
            if (!failed) {
                CCharString anim("ST_TANTRUM_OUTOF");
                failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(PBrain->GetPCreature(), anim, CSoundPair(),
                                                                                     false, true, false, 0, 0), false) != 2;
            }
        }
        if (failed) {
            Terminate();
            return;
        }
    }
    Terminate();
}