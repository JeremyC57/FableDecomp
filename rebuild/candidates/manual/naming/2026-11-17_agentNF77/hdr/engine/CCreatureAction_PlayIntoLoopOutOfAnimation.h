#ifndef FABLE_ENGINE_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_H
#define FABLE_ENGINE_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_H
// CCreatureAction_PlayIntoLoopOutOfAnimation (new shared header, agentNF77, row 0x0093A7A0 CAIStateGroup_BanditSitRoundCampfire::Singsong):
// the action that plays an "into" animation, loops a second one for a count or a time, then plays an "out of" animation.
// RTTI .?AVCCreatureAction_PlayIntoLoopOutOfAnimation@@ : CCreatureActionBase (ghidra_out/rtti_bases.tsv); vtable 0x01273B2C (slot 3
// 0x0084A850, slot 4 0x00843C10; rebuild/manifest/rtti-vtables.tsv). Members: Ego_r.pdb (struct_layouts_egor.tsv, sizeof 208 there)
// Stage +180, NumberOfTimesToLoop +184, AmountOfTimeToLoop +188, FrameToFinish +192, UseAnimationMovement +196, LoopAnimName +200,
// OutOfAnimName +204, each 0xC lower in retail (CCreatureActionBase is 0xA8 bytes here): the landed constructors 0x00842740 / 0x008427E0
// store them at +0xA8..+0xC0, and Singsong destroys the temporary's two CCharString members at +0xBC / +0xC0. Retail sizeof 0xC4
// (Singsong's temporary at [esp+14h] ends at its 0xD4-byte frame's [esp+0D8h]). Methods: Ego_r + FableWin (PerformAction FableWin only)
//   ??0..@@QAE@AAVCThingCreatureBase@@ABVCCharString@@11J_NJJ@Z   0x00842740   (count form)
//   ??0..@@QAE@AAVCThingCreatureBase@@ABVCCharString@@11M_NJJ@Z   0x008427E0   (time form)
//   ??0..@@AAE@AAVCThingCreatureBase@@ABVCCharString@@1JM_NJJ@Z   0x008428B0   (private)
//   ??0..@@AAE@AAVCThingCreatureBase@@ABVCCharString@@_NJJ@Z      0x00842960   (private)
//   ??0..@@QAE@ABV0@@Z, ??1..@@UAE@XZ, PerformAction / FinishAction (UAEXXZ), IsUsingAnimationMovement (UBE_NXZ), Clone (UBE..XZ).
// Declarations only. Needs FABLE_RETAIL_CCREATUREACTIONBASE.
// Opt-in FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR (agentNF77, row 0x0093A7A0): no user-declared destructor.
// Retail's derived CCreatureAction destructors are compiler-generated: Singsong destroys its temporary with OutOfAnimName / LoopAnimName
// expanded and then a direct call of ~CCreatureActionBase 0x00693EF0 with no vptr store (0x0093A835..0x0093A853); a user-declared
// destructor stores the vptr first (GOTCHAS 73). Default: the PDB-generated user declaration.
#ifndef FABLE_RETAIL_CCREATUREACTIONBASE
#error engine/CCreatureAction_PlayIntoLoopOutOfAnimation.h needs the retail CCreatureActionBase: define FABLE_RETAIL_CCREATUREACTIONBASE
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
class CThingCreatureBase;
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase {
public:
    enum EStage { STAGE_INTO, STAGE_LOOP, STAGE_OUT_OF };
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& creature, const CCharString& intoAnimName, const CCharString& loopAnimName,
        const CCharString& outOfAnimName, long numberOfTimesToLoop, bool useAnimationMovement, long c, long d);        // 0x00842740
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& creature, const CCharString& intoAnimName, const CCharString& loopAnimName,
        const CCharString& outOfAnimName, float amountOfTimeToLoop, bool useAnimationMovement, long c, long d);       // 0x008427E0
    CCreatureAction_PlayIntoLoopOutOfAnimation(const CCreatureAction_PlayIntoLoopOutOfAnimation&);
#ifndef FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR
    virtual ~CCreatureAction_PlayIntoLoopOutOfAnimation();
#endif
    virtual void PerformAction();
    virtual void FinishAction();
    virtual bool IsUsingAnimationMovement() const;
    virtual CCreatureActionBase* Clone() const;
private:
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& creature, const CCharString& loopAnimName, const CCharString& outOfAnimName,
        long numberOfTimesToLoop, float frameToFinish, bool useAnimationMovement, long c, long d);                   // 0x008428B0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& creature, const CCharString& animName, bool useAnimationMovement,
        long c, long d);                                                                                             // 0x00842960
protected:
    EStage      Stage;                                                       // +0xA8
    long        NumberOfTimesToLoop;                                         // +0xAC
    float       AmountOfTimeToLoop;                                          // +0xB0
    long        FrameToFinish;                                               // +0xB4
    bool        UseAnimationMovement;                                        // +0xB8
    CCharString LoopAnimName;                                                // +0xBC
    CCharString OutOfAnimName;                                               // +0xC0
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_PlayIntoLoopOutOfAnimation) == 0xC4);
#endif
#endif // FABLE_ENGINE_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_H
