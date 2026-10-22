#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    int M(int handle, int primitive, int x2);
};
int CIEngine_Methods::M(int handle, int primitive, int x2) { return 0; }