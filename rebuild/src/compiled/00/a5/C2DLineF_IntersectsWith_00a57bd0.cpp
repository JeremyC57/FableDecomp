// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
class C2DVector { public: float x; float y; };
class C2DBoxF { public: float minX; float minY; float maxX; float maxY; };
class C2DLineF {
public:
    float x1; float y1; float x2; float y2;
    bool IntersectsWith(const C2DLineF* other, C2DVector* point, float tolerance);
    int IntersectsWith(const C2DBoxF* box);
};
int C2DLineF::IntersectsWith(const C2DBoxF* box) {
    C2DVector point;
    C2DLineF bottom = { box->minX, box->minY, box->maxX, box->minY };
    C2DLineF right = { box->maxX, box->minY, box->maxX, box->maxY };
    C2DLineF top = { box->minX, box->maxY, box->maxX, box->maxY };
    C2DLineF left = { box->minX, box->minY, box->minX, box->maxY };
    extern const float DELTA_NOTIONAL_ZERO;
    if (IntersectsWith(&bottom, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&right, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&top, &point, DELTA_NOTIONAL_ZERO) ||
        IntersectsWith(&left, &point, DELTA_NOTIONAL_ZERO)) return true;
    return false;
}