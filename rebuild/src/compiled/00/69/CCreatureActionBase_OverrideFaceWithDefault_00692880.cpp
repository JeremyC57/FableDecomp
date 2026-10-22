#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimationEntry; extern unsigned int __fastcall probe(CAnimationEntry*);
bool __fastcall OverrideFaceWithDefault(CCreatureActionBase* action){ return (probe(action->PAnimEntry_Data) >> 4) & 1; }