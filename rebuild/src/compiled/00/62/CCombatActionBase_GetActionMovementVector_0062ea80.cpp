// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

class CCombatActionBase { public:
    C3DVector GetActionMovementVector();
};

C3DVector CCombatActionBase::GetActionMovementVector()
{
    return C3DVector(0.0f, 1.0f, 0.0f);
}