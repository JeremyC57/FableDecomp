// Leaf: ?AllowsCombinationAnims@CCreatureActionBase_Methods@@QAEDXZ
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CAnimationEntry {
public:
    long GetFlags() const;   // 0x005DC210
};
class CCreatureActionBase; 
struct CCreatureActionBase_Methods : CCreatureActionBase {
    char AllowsCombinationAnims();
};
char CCreatureActionBase_Methods::AllowsCombinationAnims(){ unsigned int r=(unsigned int)((int)(((CAnimationEntry*)(this->PAnimEntry_Data))->GetFlags())); return (char)((~(r>>3))&1); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ); evidence: RTTI vtable slot + PDB name set.
