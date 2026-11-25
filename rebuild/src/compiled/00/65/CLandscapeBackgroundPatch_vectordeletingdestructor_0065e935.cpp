#pragma optimize("s", on)
// Leaf: ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming (NF423, 2026-11-25): the member release 0x0065D5A0 now binds its PDB spelling ??1?$CCountedPointer@VCVillageTaskHorn@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the object through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?AutoTinyScalarDestructor_0065d5a0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CVillageTaskHorn;
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    static_cast<CCountedPointer<CVillageTaskHorn>*>(self)->CCountedPointer<CVillageTaskHorn>::~CCountedPointer();
    if (flags & 1)
        operator delete(self);
    return self;
}