#include "engine/CAIStateGroup_WorkSawingWood.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WorkSawingWood_Methods : CAIStateGroup_WorkSawingWood {
    void M(int world, int creature);
};
void CAIStateGroup_WorkSawingWood_Methods::M(int world, int creature){ this->ActionPointIndex = 0u; this->TookActionPoint = 0; }