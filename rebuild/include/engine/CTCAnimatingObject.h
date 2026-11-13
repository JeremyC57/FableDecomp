#ifndef FABLE_ENGINE_CTCANIMATINGOBJECT_H
#define FABLE_ENGINE_CTCANIMATINGOBJECT_H
// NEW (agentNF11, row 0x007F8F30 CTCAnimatingObject::PlayDefaultAnimation): the animating-object component. RTTI
// .?AVCTCAnimatingObject@@ : CTCBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CTCAnimatingObject (struct_layouts_egor.tsv):
// Animating +0x0c, CurrentAnimationPriority +0x10, QueuedAnims +0x14 (list<CAnimInit>; STLport in retail, one head-node word: the row
// clears it through `mov eax,[esi+14h]`, 0x007F8F94). Methods: the Ego_r.pdb / FableWin.pdb spellings the rows define or call
// (?PlayDefaultAnimation@CTCAnimatingObject@@QAEXXZ public; ?PlayAnimation@CTCAnimatingObject@@AAEXABVCAnimInit@@J@Z private, out of
// line at 0x007F8DC0, landed). Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_STLP_LIST and engine/CAnimInit.h (FABLE_RETAIL_CANIMINIT_CTORS).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CAnimInit.h"
#include "stlport/fable_containers.h"
class CTCAnimatingObject : public CTCBase {
public:
    void PlayDefaultAnimation();                                         // 0x007F8F30
#ifdef FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION_NAME
    // wf22 row 0x007E1FC0 CTCAmbientBird::FrameUpdate: Ego_r.pdb ?PlayAnimation@CTCAnimatingObject@@QAEXABVCCharString@@J_N@Z (public).
    void PlayAnimation(const CCharString& name, long priority, bool looping);   // 0x007F90B0
#endif
private:
    void PlayAnimation(const CAnimInit& init, long priority);            // 0x007F8DC0
    bool                 Animating;                                      // +0x0c
    long                 CurrentAnimationPriority;                       // +0x10
    std::list<CAnimInit> QueuedAnims;                                    // +0x14
#ifdef FABLE_RETAIL_CTCANIMATINGOBJECT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004DE87E
public:
#endif
#ifdef FABLE_RETAIL_CTCANIMATINGOBJECT_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void OnInitialActivate();                                    // slot 6, 0x007F90A0 (tail call to PlayDefaultAnimation)
    virtual void BulletTimeFrameUpdate();                                // slot 10, 0x007F8BD0 (the slot-9 FrameUpdate)
#endif // FABLE_RETAIL_CTCANIMATINGOBJECT_VSLOTS2
};
#endif
