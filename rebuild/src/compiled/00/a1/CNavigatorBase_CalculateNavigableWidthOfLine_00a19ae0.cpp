// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/CNavigatorBase.h"

#include "engine/C3DVector.h"

struct CNavigatorBase_Methods : CNavigatorBase {
    void CalculateNavigableWidthOfLine(C3DVector const& a, C3DVector const& pos,
                                       float& d, float& e);
};

void CNavigatorBase_Methods::CalculateNavigableWidthOfLine(C3DVector const& a,
        C3DVector const& pos, float& d, float& e)
{
    (void)a; (void)pos;
    e = 0.0f;
    d = 0.0f;
}
