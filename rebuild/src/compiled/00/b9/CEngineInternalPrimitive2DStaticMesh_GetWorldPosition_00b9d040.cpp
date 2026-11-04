// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x108];
    C3DVector m_worldPos;
};

// this-call: self in ecx, out-vector& as the single stack arg.
// Model with a dummy edx param so the real arg lands on the stack.
bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetWorldPosition(
    CEngineInternalPrimitive2DStaticMesh* self, int edx_dummy, C3DVector& out)
{
    (void)edx_dummy;
    out = self->m_worldPos;
    return true;
}