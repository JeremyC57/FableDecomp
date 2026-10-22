#include "engine/CAnimComponentFlags.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentFlags_Methods : CAnimComponentFlags {
    void SetDefaultValue();
};
void CAnimComponentFlags_Methods::SetDefaultValue(){ this->Flags = 0u; }