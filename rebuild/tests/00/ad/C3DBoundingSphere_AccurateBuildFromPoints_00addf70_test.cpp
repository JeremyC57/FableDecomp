// Behaviour test for 0x00ADDF70 C3DBoundingSphere::AccurateBuildFromPoints(const vector<C3DVector>&, float) -- forwards (begin, size, tolerance)
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct TVector { C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const C3DVector& operator[](unsigned int n) const { return _M_start[n]; } };
class C3DBoundingSphere { public:
    const C3DVector* LastPoints; long LastCount; float LastTolerance;
    void AccurateBuildFromPoints(const C3DVector* points, long n, float tol) { LastPoints = points; LastCount = n; LastTolerance = tol; }
    void AccurateBuildFromPoints(const TVector& points, float tol) { AccurateBuildFromPoints(&points[0], points.size(), tol); }
};
int main()
{
    C3DVector pts[5];
    TVector v; v._M_start = pts; v._M_finish = pts + 5; v._M_end = pts + 5;
    C3DBoundingSphere s;
    s.AccurateBuildFromPoints(v, 0.001f);
    if (s.LastPoints != pts || s.LastCount != 5 || s.LastTolerance != 0.001f) { std::printf("00addf70_TEST FAIL\n"); return 1; }
    std::printf("00addf70_TEST PASS\n");
    return 0;
}