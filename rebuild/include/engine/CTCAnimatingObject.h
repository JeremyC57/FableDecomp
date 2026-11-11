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
private:
    void PlayAnimation(const CAnimInit& init, long priority);            // 0x007F8DC0
    bool                 Animating;                                      // +0x0c
    long                 CurrentAnimationPriority;                       // +0x10
    std::list<CAnimInit> QueuedAnims;                                    // +0x14
};
#endif
