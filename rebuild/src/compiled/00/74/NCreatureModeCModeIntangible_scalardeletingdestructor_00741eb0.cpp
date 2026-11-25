#pragma optimize("t", on)
// Naming (NF423, 2026-11-25): the member release 0x0048C5C0 now binds its PDB spelling ??1?$CCountedPointer@VCCreatureActionBase@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructMember_0048c5c0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
extern void __fastcall CModeIntangible_Destruct(void* mode);
void* __fastcall CModeIntangible_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<CCreatureActionBase>*>(static_cast<unsigned char*>(mode) + 60)->CCountedPointer<CCreatureActionBase>::~CCountedPointer();
    CModeIntangible_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}