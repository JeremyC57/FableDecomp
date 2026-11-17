#ifndef FABLE_ENGINE_CTCFORCEPUSHED_H
#define FABLE_ENGINE_CTCFORCEPUSHED_H
// New header (e05 egod lane, row 0x00798940 CTCForcePushed::FrameUpdate). CTCForcePushed : CTCBase (RTTI), interface
// TCI_FORCE_PUSHED (141). Members and order from the Ego_d.pdb class record (field list 0x31E5C; the same names and offsets in
// Ego_r.pdb struct_layouts_egor.tsv): PThingCausingForcePush +0x0C, PParticle +0x14, PositionForcePushStarted +0x1C,
// DistanceBeingPushed +0x28, DirectionBeingPushed +0x2C, DistanceMovingPerFrame +0x38, Initialised +0x3C, StartedMoving +0x3D,
// PDef +0x40, Emitters_created +0x44 (STLport vector, 0xC in retail: sizeof 0x50; Ego_d 88 with VC10's 0x10 vector). Retail
// evidence: the landed SetUpForcePush 0x007986A0 / FinishBeingForcePushed 0x00798770 read the same offsets. Methods: Ego_d access
// and Ego_r.pdb spellings (?FrameUpdate@CTCForcePushed@@UAEXXZ, ?SetUpForcePush@CTCForcePushed@@QAEXABVC3DVector@@MAAVCThing@@@Z,
// ?FinishBeingForcePushed@CTCForcePushed@@AAEX_NM@Z, ?FinalCleanup@CTCForcePushed@@AAEXXZ, ...); the compiler-generated copy
// constructor and operator= of the Ego_d record are not declared. Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CINTELLIGENTPOINTER and
// the CDefPointer / C3DVector headers' retail branches.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CSpecialAbilitiesForcePushDef;
class CTCForcePushed : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FORCE_PUSHED; }
    virtual ETCInterfaceType GetInterfaceType() const;                   // 0x00798C80
    static CTCBase* FABLE_FASTCALL Alloc(CThing& thing);
    static CCharString FABLE_FASTCALL StatGetClassTypeName();
    virtual CCharString GetClassTypeName() const;                        // 0x00798C90
    CTCForcePushed(CThing& thing);                                       // 0x00798570
    virtual ~CTCForcePushed();                                           // 0x00798620
    virtual bool HasFrameUpdate() const;                                 // 0x00798CB0
    virtual void FrameUpdate();                                          // 0x00798940
    virtual void OnKill();                                               // 0x00798510
    virtual void OnRemove();                                             // 0x00798540
    void SetUpForcePush(const C3DVector& direction, float distance_to_move, CThing& thing_causing_the_force_push);   // 0x007986A0
private:
    void FinishBeingForcePushed(bool hit_in_back, float damage);         // 0x00798770
    void FinalCleanup();
    CIntelligentPointer<CThing> PThingCausingForcePush;                  // +0x0C
    CIntelligentPointer<CThing> PParticle;                               // +0x14
    C3DVector PositionForcePushStarted;                                  // +0x1C
    float DistanceBeingPushed;                                           // +0x28
    C3DVector DirectionBeingPushed;                                      // +0x2C
    float DistanceMovingPerFrame;                                        // +0x38
    bool Initialised;                                                    // +0x3C
    bool StartedMoving;                                                  // +0x3D
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;               // +0x40
    std::vector<CThing*> Emitters_created;                               // +0x44
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTCForcePushed, PThingCausingForcePush) == 0x0c);
        FABLE_STATIC_ASSERT(offsetof(CTCForcePushed, DirectionBeingPushed) == 0x2c);
        FABLE_STATIC_ASSERT(offsetof(CTCForcePushed, StartedMoving) == 0x3d);
        FABLE_STATIC_ASSERT(offsetof(CTCForcePushed, PDef) == 0x40);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTCForcePushed) == 0x50);
#endif
