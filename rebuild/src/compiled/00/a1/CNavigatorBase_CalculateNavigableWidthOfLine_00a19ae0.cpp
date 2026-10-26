#include "engine/CNavigatorBase.h"

class C3DVector { public: float x, y, z; };

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
