// Behaviour test for 0x00979260 (CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList): the comparator is built from the creature position and
// the normalised creature->threat direction, and the cover list is sorted with it.
#include <cstdio>
#include <cmath>
#include <algorithm>
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y); if (l > 0) { X /= l; Y /= l; } } };
struct CThing { C3DVector pos; };
struct CCompare {
    C2DVector Origin, Direction;
    CCompare(const C3DVector& o, const C3DVector& d) : Origin(o), Direction(d) { Direction.Normalise(); }
    // larger projection along the direction first
    bool operator()(const CThing* a, const CThing* b) const
    {
        float pa = (a->pos.X - Origin.X) * Direction.X + (a->pos.Y - Origin.Y) * Direction.Y;
        float pb = (b->pos.X - Origin.X) * Direction.X + (b->pos.Y - Origin.Y) * Direction.Y;
        return pa > pb;
    }
};
static void Process(CThing* creature, CThing* threat, CThing** first, CThing** last, CCompare** seen)
{
    C3DVector d; d.X = threat->pos.X - creature->pos.X; d.Y = threat->pos.Y - creature->pos.Y; d.Z = threat->pos.Z - creature->pos.Z;
    CCompare cmp(creature->pos, d);
    static CCompare keep(cmp); keep = cmp; *seen = &keep;
    std::sort(first, last, cmp);
}
int main()
{
    CThing creature = { { 0, 0, 0 } }, threat = { { 10, 0, 5 } };
    CThing a = { { 1, 5, 0 } }, b = { { 8, -2, 0 } }, c = { { -3, 1, 0 } };
    CThing* list[3] = { &a, &b, &c };
    CCompare* cmp = 0;
    Process(&creature, &threat, list, list + 3, &cmp);
    if (list[0] != &b || list[1] != &a || list[2] != &c || cmp->Direction.X < 0.999f || cmp->Origin.X != 0.0f) {
        std::printf("00979260_TEST FAIL\n"); return 1;
    }
    std::printf("00979260_TEST PASS\n");
    return 0;
}
