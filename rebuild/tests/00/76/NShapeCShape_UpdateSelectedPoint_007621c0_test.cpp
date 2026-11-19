// Behaviour test for NShape::CShape::UpdateSelectedPoint (0x007621C0): the row body over std::vector. Points on, inside and outside the
// 0.25 2D radius (Z ignored; the compare is strict), repeated calls (each point selected once), no hit clears the selection. Kills: <=
// for <, a 3D distance, the radius, a missing uniqueness check, the success flag, the clear on a miss.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
inline float GFSqr(float n){return n*n;}
inline bool GFWorld2DDistanceSmallerThan(const C3DVector& pos1,const C3DVector& pos2,float compare,bool using_delta_notional_zero) {
 float sqr_dist=GFSqr(pos1.X-pos2.X)+GFSqr(pos1.Y-pos2.Y);
 float sqr_comp=using_delta_notional_zero?compare:GFSqr(compare);
 return sqr_dist<sqr_comp;
}
namespace NShape {
class CShape {public:
 std::vector<C3DVector> Points;
 std::vector<std::vector<C3DVector>::iterator> SelectedPoints;
 bool UpdateSelectedPoint(const C3DVector& pos);
};}
bool NShape::CShape::UpdateSelectedPoint(const C3DVector& pos)
{
    bool success = false;
    for (std::vector<C3DVector>::iterator it = Points.begin(); it != Points.end(); ++it) {
        C3DVector this_point = *it;
        if (GFWorld2DDistanceSmallerThan(this_point, pos, 0.25f, false)) {
            std::vector<std::vector<C3DVector>::iterator>::iterator it_already_there = std::find(SelectedPoints.begin(), SelectedPoints.end(), it);
            if (it_already_there == SelectedPoints.end())
                SelectedPoints.push_back(it);
            success = true;
        }
    }
    if (!success)
        SelectedPoints.clear();
    return success;
}
int main(){
 NShape::CShape s;
 s.Points.push_back(C3DVector(0.0f,0.0f,0.0f));      // 0: centre
 s.Points.push_back(C3DVector(0.25f,0.0f,0.0f));     // 1: exactly on the radius (not selected)
 s.Points.push_back(C3DVector(0.0f,0.2f,50.0f));     // 2: inside in 2D, far in Z (selected)
 s.Points.push_back(C3DVector(0.15f,0.15f,0.0f));    // 3: 0.045 < 0.0625 (selected)
 s.Points.push_back(C3DVector(0.2f,0.2f,0.0f));      // 4: 0.08 > 0.0625 (not selected)
 s.Points.push_back(C3DVector(-0.24f,0.0f,0.0f));    // 5: inside
 s.Points.reserve(16);
 std::vector<C3DVector>::iterator b=s.Points.begin();
 check(s.UpdateSelectedPoint(C3DVector(0,0,0)));
 check(s.SelectedPoints.size()==4&&s.SelectedPoints[0]==b&&s.SelectedPoints[1]==b+2&&s.SelectedPoints[2]==b+3&&s.SelectedPoints[3]==b+5);
 check(s.UpdateSelectedPoint(C3DVector(0,0,-7)));          // again: no duplicates
 check(s.SelectedPoints.size()==4);
 check(s.UpdateSelectedPoint(C3DVector(0.45f,0.0f,0)));    // only point 1 (0.04): appended
 check(s.SelectedPoints.size()==5&&s.SelectedPoints[4]==b+1);
 check(!s.UpdateSelectedPoint(C3DVector(5.0f,5.0f,0)));    // miss: cleared
 check(s.SelectedPoints.empty());
 check(!s.UpdateSelectedPoint(C3DVector(0.0f,0.5f,0)));    // 0.2 -> point 2 at 0.09 > 0.0625: miss
 check(s.SelectedPoints.empty());
 NShape::CShape e; check(!e.UpdateSelectedPoint(C3DVector(0,0,0))&&e.SelectedPoints.empty());
 puts("007621c0_TEST PASS");return 0;
}