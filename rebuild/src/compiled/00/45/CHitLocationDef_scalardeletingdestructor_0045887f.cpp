#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
// Naming (NF423, 2026-11-25): the member release 0x0044F3B2 now binds its PDB spelling ?ReleaseData@?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@QAEXXZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructMember_0044f3b2@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NHitLocations { class CPhysicalPrimitiveBase; }
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
void* __fastcall CHitLocationDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>*>(static_cast<unsigned char*>(def) + 60)->ReleaseData();
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}