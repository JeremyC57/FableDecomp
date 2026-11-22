// 00a58790 NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine
// Leaf: ?GetFirstIntersectionWithFiniteLine@CSphere@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#include "engine/CSphere.h"

namespace NBBBMaths {
bool CSphere::GetFirstIntersectionWithFiniteLine(
    const C3DVector& line_start, const C3DVector& line_end,
    C3DVector& intersection_pos, C3DVector& outward_normal) const
{
    C3DVector line_dir = line_end - line_start;
    if (!line_dir.IsNonZero())
        return false;

    C3DVector start_base = line_start - Base;
    C3DVector line_start_prime = line_start - Base;
    float a = line_dir.Dot(line_dir);
    float b = 2.0f * line_start_prime.Dot(line_dir);
    float c = line_start_prime.Dot(line_start_prime) - GFSqr(Radius);
    float desc = b * b - 4.0f * a * c;
    if (desc < 0.0f)
        return false;

    float root = GFRoot(desc);
    float lambda1 = (-b + root) / (2.0f * a);
    float lambda2 = (-b - root) / (2.0f * a);
    float best_lambda = 2.0f;
    C3DVector best_intersection_pt;
    if (lambda1 > 0.0f && lambda1 < 1.0f && lambda1 < best_lambda) {
        best_lambda = lambda1;
        best_intersection_pt = line_start + line_dir * lambda1;
    }
    if (lambda2 > 0.0f && lambda2 < 1.0f && lambda2 < best_lambda) {
        best_lambda = lambda2;
        best_intersection_pt = line_start + line_dir * lambda2;
    }
    if ((lambda1 < 0.0f && lambda2 > 1.0f) || (lambda2 < 0.0f && lambda1 > 1.0f)) {
        best_intersection_pt = line_start;
        best_lambda = 0.5f;
    }
    if (best_lambda > 0.0f && best_lambda < 1.0f) {
        intersection_pos = best_intersection_pt;
        outward_normal = intersection_pos - Base;
        outward_normal.AccurateNormalise();
        return true;
    }
    return false;
}
}

#include <stdio.h>
const float DELTA_NOTIONAL_ZERO = 0.0001f;
NBBBMaths::CSphere::CSphere(const C3DVector& base,float radius):Radius(radius),Base(base) {}
C3DVector& C3DVector::AccurateNormalise() {
    float len = (float)sqrt(X*X + Y*Y + Z*Z);
    if (len) { X/=len; Y/=len; Z/=len; }
    return *this;
}

int main() {
    struct Case {float x0,x1,y; bool reaches; float x;};
    const Case cases[] = {
        {-4,4,0,true,-2}, {4,-4,0,true,2}, {-3,-1,0,true,-2},
        {1,3,0,true,2}, {1,-1,0,true,1}, {-1,1,0,true,-1},
        {-4,4,2,true,0}, {-4,4,3,false,0}, {3,4,0,false,0},
        {-3,-2,0,false,0}, {-2,-3,0,false,0}, {2,2,0,false,0}
    };
    NBBBMaths::CSphere sphere(C3DVector(8,4,2),2.0f);
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i) {
        const Case& c=cases[i];
        C3DVector pos(19,21,23), normal(29,31,37);
        bool hit=sphere.GetFirstIntersectionWithFiniteLine(C3DVector(8+c.x0,4+c.y,2),
                      C3DVector(8+c.x1,4+c.y,2),pos,normal);
        if (c.reaches!=hit) return 1;
        if (hit) {
            const float nx=c.x==0?0.0f:(c.x>0?1.0f:-1.0f);
            if(pos.X!=8+c.x||pos.Y!=4+c.y||pos.Z!=2||normal.X!=nx||
               normal.Y!=(c.y==2?1.0f:0.0f)||normal.Z!=0) return 2;
        } else if(pos.X!=19||pos.Y!=21||pos.Z!=23||normal.X!=29||normal.Y!=31||normal.Z!=37)
            return 3;
    }
    puts("00a58790_TEST PASS");
    return 0;
}