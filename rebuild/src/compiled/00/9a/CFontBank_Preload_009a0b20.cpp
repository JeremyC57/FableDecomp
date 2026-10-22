#include "engine/CFontBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CFontBank_Methods : CFontBank {
    bool M(int text, int x1);
};
bool CFontBank_Methods::M(int text, int x1) { return true; }