// CAIStateGroup_GoToBed::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_GoToBed, then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_GoToBed_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF470 (T3 nf303 0x008FD288, callee-only respell): the call at +3 (retail 0x008FEB9A -> 0x008FD2CA) now names the real implicit destructor
//   ??1CAIStateGroup_GoToBed@@UAE@XZ on the shared retail class (engine/CAIStateGroup_GoToBed.h; Ego_r / FableWin type records flag
//   ~CAIStateGroup_GoToBed compiler-generated), not the free stand-in CAIStateGroup_GoToBed_Destruct. VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x008FD2CA (PBed +0x14, BedSide +0x30, then ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
class CThing; class CWorld; class CThingAICreature;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_GoToBed.h"
void* __fastcall CAIStateGroup_GoToBed_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_GoToBed*>(stateGroup)->CAIStateGroup_GoToBed::~CAIStateGroup_GoToBed();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
