#ifndef FABLE_ENGINE_CAISTATEGROUP_MINIONBEHAVIOURCROWDBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_MINIONBEHAVIOURCROWDBASE_H
// New header (chunk Q16, row 0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update). Ego_r.pdb CAIStateGroup_MinionBehaviourCrowdBase
// (0x3C, struct_layouts_egor.tsv): PCrowdThing CIntelligentPointer<const CThing> +0x14, PTaskCrowd CIntelligentPointer<CVillageTaskCrowd> +0x1C,
// MyMessageEvent CIntelligentPointer<const CMessageEvent> +0x24, DesignatedCrowdPosition C3DVector +0x2C, IsPartOfCrowd +0x38.
// RTTI .?AVCAIStateGroup_MinionBehaviourCrowdBase@@ : CAIStateGroupBase. Name set (Ego_r / FableWin): ctor, ~ (FableWin), Initialise,
// Cleanup, BannerPaintText (IBE), IsRunnable, Update. Retail vtable 0x0128C494: slots 0-13 the base's, slots 14-17 _purecall; the
// derived CAIStateGroup_MinionBehaviourPodiumEmoteNeutral (vtable 0x0128D16C) fills them with IsRunnableLeaf (0x0096F4A0 `mov al,1`),
// UpdateLeaf (0x0096F570), CleanupLeaf (0x0096F510), InitLeaf (0x0096F490) -- the Ego_r names of that class (UAE, public).
// Needs FABLE_RETAIL_CAISTATEGROUPBASE + FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS + FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
class CThing;
class CVillageTaskCrowd;
class CMessageEvent;
class CAIStateGroup_MinionBehaviourCrowdBase : public CAIStateGroupBase {
public:
    CAIStateGroup_MinionBehaviourCrowdBase();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
                            const CCharString& name);                    // slot 1  0x00957BB0
    virtual void Cleanup();                                              // slot 9  0x00957BE0
    virtual bool IsRunnable();                                           // slot 11 0x00957C30
    virtual void Update();                                               // slot 12 0x00957DB0
    virtual bool IsRunnableLeaf() = 0;                                   // slot 14
    virtual void UpdateLeaf() = 0;                                       // slot 15
    virtual void CleanupLeaf() = 0;                                      // slot 16
    virtual void InitLeaf() = 0;                                         // slot 17
protected:
    void BannerPaintText(const CCharString& text) const;
    CIntelligentPointer<const CThing> PCrowdThing;                       // +0x14
    CIntelligentPointer<CVillageTaskCrowd> PTaskCrowd;                   // +0x1C
    CIntelligentPointer<const CMessageEvent> MyMessageEvent;             // +0x24
    C3DVector DesignatedCrowdPosition;                                   // +0x2C
    bool IsPartOfCrowd;                                                  // +0x38
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionBehaviourCrowdBase) == 0x3C);
#endif
