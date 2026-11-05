// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"

class CEngineInternalPrimitiveDecal { public:
    char pad[0x54];
    C3DVector m_worldPosition;

    bool GetWorldPosition(C3DVector& out);
};

bool CEngineInternalPrimitiveDecal::GetWorldPosition(C3DVector& out)
{
    out = m_worldPosition;
    return true;
}