// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

class CTCCoopSpirit { public:
    char pad[0x154];
    C3DVector movement;
};

// Retail is __fastcall: this=ecx, source ptr on stack, ret 4.
// Emulate with __fastcall + an unused edx slot so the real arg stays on the stack.
void __fastcall ApplyMovementVector(CTCCoopSpirit* self, int /*edx_unused*/, const C3DVector* pos)
{
    self->movement = *pos;
}