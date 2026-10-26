#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CAnimationEntry {
public:
    long GetFlags() const;   // 0x005DC210
};
class CCreatureActionBase; 
struct CCreatureActionBase_Methods : CCreatureActionBase {
    char M();
};
char CCreatureActionBase_Methods::M(){ unsigned int r=(unsigned int)((int)(((CAnimationEntry*)(this->PAnimEntry_Data))->GetFlags())); return (char)((~(r>>3))&1); }