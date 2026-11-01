// C2DLineF::IntersectsWith(C2DBoxI*) -- convert integer box corners to float edges and test each using the shared segment intersection helper
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
class C2DVector { public: float x; float y; };
class C2DBoxI { public: long minX; long minY; long maxX; long maxY; };
class C2DLineF { public:
    float x1; float y1; float x2; float y2;
    C2DLineF(float ax,float ay,float bx,float by):x1(ax),y1(ay),x2(bx),y2(by) {}
    bool IntersectsWith(const C2DLineF* other, C2DVector* point, float tolerance); // 0x00A56C80
    int IntersectsWith(const C2DBoxI* box);
};
extern const float DELTA_NOTIONAL_ZERO;             // 0x0129BA3C
int C2DLineF::IntersectsWith(const C2DBoxI* box)
{
    C2DVector point;
    C2DLineF bottom((float)box->minX, (float)box->minY, (float)box->maxX, (float)box->minY);
    C2DLineF right((float)box->maxX, (float)box->minY, (float)box->maxX, (float)box->maxY);
    C2DLineF top((float)box->minX, (float)box->maxY, (float)box->maxX, (float)box->maxY);
    C2DLineF left((float)box->minX, (float)box->minY, (float)box->minX, (float)box->maxY);
    if (IntersectsWith(&bottom, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&right, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&top, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&left, &point, DELTA_NOTIONAL_ZERO)) return true;
    return false;
}