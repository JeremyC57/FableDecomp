#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimationEntry; extern int __fastcall cat_probe(CAnimationEntry*);
bool __fastcall CanAnimationTransitionOut(CCreatureActionBase* action)
{
    return (~cat_probe(action->PAnimEntry_Data)) & 1;
}