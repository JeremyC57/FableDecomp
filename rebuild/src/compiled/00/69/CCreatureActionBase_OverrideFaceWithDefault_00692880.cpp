#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CAnimationEntry {
public:
    long GetFlags() const;   // 0x005DC210
};
bool __fastcall OverrideFaceWithDefault(CCreatureActionBase* action){ return (((unsigned int)(((CAnimationEntry*)(action->PAnimEntry_Data))->GetFlags())) >> 4) & 1; }