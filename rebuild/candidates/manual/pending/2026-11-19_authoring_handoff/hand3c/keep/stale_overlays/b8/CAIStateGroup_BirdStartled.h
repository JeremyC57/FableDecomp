#ifndef FABLE_ENGINE_CAISTATEGROUP_BIRDSTARTLED_H
#define FABLE_ENGINE_CAISTATEGROUP_BIRDSTARTLED_H
// CAIStateGroup_BirdStartled (new shared header, hand3c, row 0x00914650 IsRunnable). RTTI .?AVCAIStateGroup_BirdStartled@@ :
// CAIStateGroup_FlyingThingBase : CAIStateGroupBase : CBaseClass (ghidra_out/rtti_bases.tsv); vtable 0x012896C0 (rtti-vtables:
// slot 0 ??_G 0x008FF932, 11 IsRunnable 0x00914650, 12 Update 0x00914600). Ego_r.pdb / FableWin.pdb ?IsRunnable@..@@UAE_NXZ,
// ?Update@..@@UAEXXZ (public overrides); no data members of its own (Ego_r has no record beyond the 0x24-byte base).
// Needs FABLE_RETAIL_CAISTATEGROUPBASE (the polymorphic base).
#include "engine/CAIStateGroup_FlyingThingBase.h"
class CAIStateGroup_BirdStartled : public CAIStateGroup_FlyingThingBase {
public:
    virtual bool IsRunnable();                                           // slot 11 0x00914650
    virtual void Update();                                               // slot 12 0x00914600
};
#endif // FABLE_ENGINE_CAISTATEGROUP_BIRDSTARTLED_H
