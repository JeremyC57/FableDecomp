// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CThing;

struct CGlob {
    char pad[0x24];
    int m24;
};

extern CGlob* g_glob;

#include "engine/C3DVector.h"

C3DVector __fastcall Inner(int a, CThing* t, float f);

C3DVector __fastcall GetCreatureFireAtPos(CThing* thing)
{
    return Inner(g_glob->m24, thing, 1.0f);
}