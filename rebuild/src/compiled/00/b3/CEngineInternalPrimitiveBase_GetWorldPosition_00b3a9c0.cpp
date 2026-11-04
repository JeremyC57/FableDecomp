// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

bool __fastcall CEngineInternalPrimitiveBase_GetWorldPosition(void* primitive, int edx, C3DVector* out)
{
    (void)primitive;
    (void)edx;
    (void)out;
    return false;
}