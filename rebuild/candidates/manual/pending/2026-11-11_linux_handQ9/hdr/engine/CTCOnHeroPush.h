#ifndef FABLE_ENGINE_CTCONHEROPUSH_H
#define FABLE_ENGINE_CTCONHEROPUSH_H
// CTCOnHeroPush (chunk Q9, row 0x0092B900 CAIStateGroup_OnHeroPush::Update; new header): the component a creature carries while the
// hero pushes it (interface TCI_ON_HERO_PUSH 196; added by name "CTCOnHeroPush" in CThingAICreature::OnHeroPush 0x008328D0).
// Layout: Ego_r.pdb CTCOnHeroPush (struct_layouts_egor.tsv, 32 bytes, CTCBase 0xC): DistPushed +0x0C, PushDir +0x10,
// FrameSincePushed +0x1C; retail matches (0x008328D0 writes +0x0C/+0x10/+0x1C, 0x0092B900 reads PushDir +0x10 and
// FrameSincePushed +0x1C). Members only: no row here reaches its methods.
#ifdef FABLE_RETAIL_CTCBASE   // the polymorphic retail CTCBase model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CTCBase.h"
class CTCOnHeroPush : public CTCBase {
public:
    float     DistPushed;                                                // +0x0C
    C3DVector PushDir;                                                   // +0x10
    long      FrameSincePushed;                                          // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CTCOnHeroPush) == 0x20);
#endif // FABLE_RETAIL_CTCBASE
#endif
