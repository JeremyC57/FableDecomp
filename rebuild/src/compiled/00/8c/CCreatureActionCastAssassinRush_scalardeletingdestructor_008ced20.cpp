#pragma optimize("t", on)
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
// Naming (NF423, 2026-11-25): the member release 0x0048C5C0 now binds its PDB spelling ??1?$CCountedPointer@VCCreatureActionBase@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructMember_0048c5c0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
void* __fastcall CCreatureAction_CastAssassinRush_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<CCreatureActionBase>*>(static_cast<unsigned char*>(action) + 300)->CCountedPointer<CCreatureActionBase>::~CCountedPointer();
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}