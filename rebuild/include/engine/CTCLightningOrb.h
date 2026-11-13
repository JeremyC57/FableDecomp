#ifndef FABLE_ENGINE_CTCLIGHTNINGORB_H
#define FABLE_ENGINE_CTCLIGHTNINGORB_H
// New shared header (w22 agent2022, row 0x007F26B0 CTCLightningOrb::Initialise): the summoner's lightning orb component.
// RTTI .?AVCTCLightningOrb@@ : CTCBase; vtable 0x01240A2C (rebuild/manifest/rtti-vtables.tsv): 6 OnInitialActivate 0x007F23D0, 8 HasFrameUpdate
// 0x004DB540, 9 FrameUpdate 0x007F24E0, 13 OnDie 0x007F2410, 21 GetInterfaceType 0x004DB52A, 22 GetClassTypeName 0x004DB530 (names: Ego_r.pdb /
// FableWin.pdb). Methods: Initialise(CThing&, CThing&) public 0x007F26B0, SetLifeTime(float), SetPParent(CThing&) / SetPTarget(CThing&) (FableWin.pdb
// only: inline in Ego_r and retail; Initialise 0x007F26BC..0x007F2735 expands both: a temporary CIntelligentPointer<CThing>, then GetPItem /
// SetPItem into the member). Members: Ego_r.pdb names and order at the Ego_r offsets (no container): PParent +0x0C, PTarget +0x14, PDef
// CDefPointer<const CLightningOrbDef> +0x1C, Timer +0x20, LifeTime +0x24, NumFramesForSplineTraversal +0x28, PSpline
// CCountedPointer<NBBBMaths::CLinearSpline> +0x2C; sizeof 0x34 (the landed FrameUpdate 0x007F24E0 / OnInitialActivate 0x007F23D0 offsets).
// Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CINTELLIGENTPOINTER(_ASSIGN), FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CLightningOrbDef.h"
#include "engine/NBBBMaths_CLinearSpline.h"
class CThing;

class CTCLightningOrb : public CTCBase {
public:
    CTCLightningOrb(CThing&);                                            // ??0CTCLightningOrb@@QAE@AAVCThing@@@Z
    virtual ~CTCLightningOrb();
    virtual void OnInitialActivate();                                    //  6 0x007F23D0
    virtual bool HasFrameUpdate() const;                                 //  8 0x004DB540
    virtual void FrameUpdate();                                          //  9 0x007F24E0
    virtual void OnDie();                                                // 13 0x007F2410
    virtual ETCInterfaceType GetInterfaceType() const;                   // 21 0x004DB52A
    virtual CCharString GetClassTypeName() const;                        // 22 0x004DB530
    void Initialise(CThing& parent, CThing& target);                     // ?Initialise@CTCLightningOrb@@QAEXAAVCThing@@0@Z 0x007F26B0
    void SetLifeTime(float life_time);                                   // ?SetLifeTime@CTCLightningOrb@@QAEXM@Z
    void SetPParent(CThing& parent) { PParent = &parent; }               // ?SetPParent@CTCLightningOrb@@QAEXAAVCThing@@@Z (inline)
    void SetPTarget(CThing& target) { PTarget = &target; }               // ?SetPTarget@CTCLightningOrb@@QAEXAAVCThing@@@Z (inline)
    CIntelligentPointer<CThing>                 PParent;                 // +0x0c
    CIntelligentPointer<CThing>                 PTarget;                 // +0x14
    CDefPointer<const CLightningOrbDef>         PDef;                    // +0x1c
    long                                        Timer;                   // +0x20
    long                                        LifeTime;                // +0x24
    long                                        NumFramesForSplineTraversal;   // +0x28
    CCountedPointer<NBBBMaths::CLinearSpline>   PSpline;                 // +0x2c
};
FABLE_STATIC_ASSERT(offsetof(CTCLightningOrb, PTarget) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCLightningOrb, PDef) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCLightningOrb, PSpline) == 0x2c);
FABLE_STATIC_ASSERT(sizeof(CTCLightningOrb) == 0x34);
#endif
