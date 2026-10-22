#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    unsigned long M(int texture, int name);
};
unsigned long CIEngine_Methods::M(int texture, int name) { return 0; }