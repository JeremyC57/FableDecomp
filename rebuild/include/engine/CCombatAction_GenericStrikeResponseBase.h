#ifndef FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_H
#define FABLE_ENGINE_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_H
#include "engine/CCombatActionBase.h"
// Original TPI0x3bfa3; native setters at134/135 and ctor size148 prove full shape.
class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif
public:
    virtual ~CCombatAction_GenericStrikeResponseBase();
#ifdef FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR
    // Retail opt-in (wfE11, row 0x006F5C10): the default constructor, out of line in retail at 0x00867950 (called with no argument by the
    // derived constructors expanded in CCreatureActionBase::StatRegisterClass and by the landed 0x006F5790). Declaration only.
    CCombatAction_GenericStrikeResponseBase();                          // 0x00867950
#endif
    void SetMoveOnHitOverride(bool b) { MoveOnHitOverride=b; }
    void SetCreateDustEffect(bool b) { CreateDustEffect=b; }
#ifdef FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED
    // Retail opt-in (wf20, row 0x008C12D0 CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise): Ego_r.pdb
    // ?Initialise@CCombatAction_GenericStrikeResponseBase@@UAEXXZ (0x00865F40, called directly by the derived Initialise) and the
    // Ego_r.pdb member types PThingThatHit CIntelligentPointer<const CThing> (+0x114 retail), PointOfHit / DirectionOfHit C3DVector
    // (+0x11C / +0x128). Same layout as the opaque default. Needs engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER),
    // engine/C3DVector.h and CCombatActionBase's Initialise declaration (FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER) first.
public:
    virtual void Initialise();                                           // 0x00865F40
#ifdef FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_FINISHACTION
    // NF74 opt-in (row 0x008C2550 CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction): Ego_r.pdb / FableWin.pdb
    // ?FinishAction@CCombatAction_GenericStrikeResponseBase@@UAEXXZ (0x008663D0, called directly by the derived FinishAction).
    virtual void FinishAction();                                         // 0x008663D0
#endif
protected:
    CIntelligentPointer<const CThing> PThingThatHit;                     // +0x114
    C3DVector PointOfHit,DirectionOfHit;                                 // +0x11C, +0x128
#else
protected:
    unsigned char PThingThatHit[8],PointOfHit[12],DirectionOfHit[12];
#endif
    bool MoveOnHitOverride,CreateDustEffect;
    long FrameForDust;
    unsigned char DustTravelDirection[12];
};
FABLE_STATIC_ASSERT_AT(32, sizeof(CCombatAction_GenericStrikeResponseBase)==0x148);
#endif
