#ifndef FABLE_ENGINE_CTCPHYSICSCONTROLLED_H
#define FABLE_ENGINE_CTCPHYSICSCONTROLLED_H
// New header (chunk 2004, row 0x00725500 CTCPhysicsControlled::PerformImpulseBasedMovement). Retail opt-in only:
// FABLE_RETAIL_CTCPHYSICSCONTROLLED (needs FABLE_RETAIL_CTCPHYSICSSTANDARD, FABLE_RETAIL_CTCPHYSICSBASE, FABLE_RETAIL_CTCBASE).
// Class: Fable.exe RTTI .?AVCTCPhysicsControlled@@ (vtable 0x0126616C) : CTCPhysicsStandard (ghidra_out/rtti_bases.tsv), class key V.
// Members: Ego_r.pdb TPI (struct_layouts_egor.tsv, 172 bytes; CTCPhysicsStandard ends at 0x88 in retail and in the PDB):
// ImpulseVelocity +0x88 (the row's `fld [esi+88h] / [esi+8Ch] / [esi+90h]`), ForcedMoveToPos +0x94, the two bitfields +0xA0,
// RenderZOffset +0xA4, LastRenderZOffset +0xA8. Method: Ego_r.pdb / FableWin.pdb
// ?PerformImpulseBasedMovement@CTCPhysicsControlled@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@AAVC3DVector@@@Z
// (public, non-virtual). The class's own virtual overrides are not declared (the row calls only CTCPhysicsBase slots).
#ifdef FABLE_RETAIL_CTCPHYSICSCONTROLLED
#include "rebuild_abi.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCPhysicsControlled : public CTCPhysicsStandard {
public:
    void PerformImpulseBasedMovement(std::vector<CThing*>& things, C3DVector& pos);   // 0x00725500
#ifdef FABLE_RETAIL_CTCPHYSICSCONTROLLED_MOVE
    // Retail opt-in (agentNF55, row 0x00725C40): Ego_r.pdb ?Move@CTCPhysicsControlled@@UAEXXZ (the CTCPhysicsBase::Move
    // override) and the two private steps it calls, ?AdjustThingListForDebugHelpers@CTCPhysicsControlled@@AAEXAAV?$vector@
    // PAVCThing@@...@@Z (0x007254B0) and ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ (0x00725B70). Declarations only.
    virtual void Move();
private:
    void AdjustThingListForDebugHelpers(std::vector<CThing*>& things);                 // 0x007254B0
    void UpdateAdditionalImpulse();                                                     // 0x00725B70
#endif
private:
    C3DVector ImpulseVelocity;                                           // +0x88
    C3DVector ForcedMoveToPos;                                           // +0x94
    bool ForcedMoveToPosSet:1;                                           // +0xa0
    bool ForcedMoveToPosUsePhysics:1;
    float RenderZOffset;                                                 // +0xa4
    float LastRenderZOffset;                                             // +0xa8
};
FABLE_STATIC_ASSERT_AT(28, sizeof(CTCPhysicsControlled) == 0xac);
#endif
#endif // FABLE_ENGINE_CTCPHYSICSCONTROLLED_H
