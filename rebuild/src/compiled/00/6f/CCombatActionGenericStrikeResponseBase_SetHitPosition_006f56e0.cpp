// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

class CCombatAction_GenericStrikeResponseBase { public:
    char pad[0x11c];
    C3DVector hitPos;

    void SetHitPosition(C3DVector pos);
};

void CCombatAction_GenericStrikeResponseBase::SetHitPosition(C3DVector pos)
{
    this->hitPos = pos;
}