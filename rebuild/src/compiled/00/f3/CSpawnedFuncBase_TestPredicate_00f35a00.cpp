// Leaf: ?CSpawnedFuncBase_TestPredicate@@YI_NPAVCSpawnedFuncBase@@@Z
#include "engine/CSpawnedFuncBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPredicateBase { virtual bool v0(); };
bool __fastcall CSpawnedFuncBase_TestPredicate(CSpawnedFuncBase* process){ CPredicateBase* p = process->PPredicate_Data; if(!p) return true; return p->v0(); }