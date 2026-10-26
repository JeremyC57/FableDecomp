// C2DLineF::IntersectsWith(C2DBoxI*) -- convert integer box corners to float edges and test each using the shared segment intersection helper
class C2DVector { public: float x; float y; };
class C2DBoxI { public: long minX; long minY; long maxX; long maxY; };
class C2DLineF { public:
    float x1; float y1; float x2; float y2;
    C2DLineF(float ax,float ay,float bx,float by):x1(ax),y1(ay),x2(bx),y2(by) {}
    bool IntersectsWith(const C2DLineF* other, C2DVector* point, float tolerance); // 0x00A56C80
    int IntersectsWith(const C2DBoxI* box);
};
extern float g_C2DIntersectionTolerance; // 0x0129BA3C
int C2DLineF::IntersectsWith(const C2DBoxI* box)
{
    C2DVector point;
    C2DLineF bottom((float)box->minX, (float)box->minY, (float)box->maxX, (float)box->minY);
    C2DLineF right((float)box->maxX, (float)box->minY, (float)box->maxX, (float)box->maxY);
    C2DLineF top((float)box->minX, (float)box->maxY, (float)box->maxX, (float)box->maxY);
    C2DLineF left((float)box->minX, (float)box->minY, (float)box->minX, (float)box->maxY);
    if (IntersectsWith(&bottom, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&right, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&top, &point, g_C2DIntersectionTolerance) ||
        IntersectsWith(&left, &point, g_C2DIntersectionTolerance)) return true;
    return false;
}