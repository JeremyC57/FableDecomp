// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

class CCombatAction_GenericStrikeResponseBase { public:
    char pad[0x128];
    C3DVector m_hitDir;
    void SetHitDirection(C3DVector v);
};

void CCombatAction_GenericStrikeResponseBase::SetHitDirection(C3DVector v)
{
    m_hitDir = v;
}