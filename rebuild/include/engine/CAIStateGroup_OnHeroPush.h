#ifndef FABLE_ENGINE_CAISTATEGROUP_ONHEROPUSH_H
#define FABLE_ENGINE_CAISTATEGROUP_ONHEROPUSH_H
// CAIStateGroup_OnHeroPush (chunk Q9, row 0x0092B900 Update; new header): the AI state group that turns a pushed creature along
// the push and plays its pushed animation. Layout: Ego_r.pdb CAIStateGroup_OnHeroPush (struct_layouts_egor.tsv, 32 bytes):
// PushDir +0x14 after the 0x14-byte CAIStateGroupBase; retail matches (0x0092B942 `lea esi,[edi+14h]`). Only the row's method is
// declared (Ego_r.pdb ?Update@CAIStateGroup_OnHeroPush@@UAEXXZ, virtual).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CAIStateGroupBase.h"
class CAIStateGroup_OnHeroPush : public CAIStateGroupBase {
public:
    virtual void Update();                                               // 0x0092B900
private:
    C3DVector PushDir;                                                   // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_OnHeroPush) == 0x20);
#endif
