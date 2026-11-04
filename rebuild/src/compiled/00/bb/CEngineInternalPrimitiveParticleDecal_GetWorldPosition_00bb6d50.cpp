// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

struct CEngineInternalPrimitiveParticleDecal {
    char pad[0x48];
    C3DVector m_pos;

    bool GetWorldPosition(C3DVector& out);
};

bool CEngineInternalPrimitiveParticleDecal::GetWorldPosition(C3DVector& out)
{
    out = m_pos;
    return true;
}