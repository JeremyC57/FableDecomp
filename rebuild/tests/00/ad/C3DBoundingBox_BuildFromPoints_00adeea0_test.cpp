// Behaviour test for 0x00ADEEA0 C3DBoundingBox::BuildFromPoints(const vector<C3DVector>&) -- forwards (begin, (float)size)
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct TVector { C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const C3DVector& operator[](unsigned int n) const { return _M_start[n]; } };
class C3DBoundingBox { public:
    const C3DVector* LastPoints; float LastCount;
    void BuildFromPoints(const C3DVector* points, float n) { LastPoints = points; LastCount = n; }
    void BuildFromPoints(const TVector& points) { BuildFromPoints(&points[0], (float)points.size()); }
};
int main()
{
    C3DVector pts[7];
    TVector v; v._M_start = pts; v._M_finish = pts + 7; v._M_end = pts + 7;
    C3DBoundingBox b;
    b.BuildFromPoints(v);
    if (b.LastPoints != pts || b.LastCount != 7.0f) { std::printf("00adeea0_TEST FAIL\n"); return 1; }
    v._M_finish = pts;
    b.BuildFromPoints(v);
    if (b.LastCount != 0.0f) { std::printf("00adeea0_TEST FAIL\n"); return 1; }
    std::printf("00adeea0_TEST PASS\n");
    return 0;
}