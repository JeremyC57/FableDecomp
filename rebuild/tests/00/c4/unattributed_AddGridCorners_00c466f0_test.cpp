#include <stdio.h>
#ifndef _MSC_VER
#define __declspec(x) __attribute__((aligned(8)))
#define __fastcall
#endif
// _unattributed::AddGridCorners(const C2DCoordI*) -- intern four corners of a scaled grid coordinate
// Retail 0x00C466F0, 218 bytes. Propagated CWorld event-handler label is contradicted by every callee.
// 00C620B0 converts two integer coordinates to float; 00C46F20 scales a vector;
// 00C46E70 adds vectors; 00C465F0 interns a vector+length and marks the first corner.
// No canonical header identifies this owner. Field +4 is a vector-interning interface, +8 the grid step.
// Eight-byte stack alignment and all six vector slots at eight-byte boundaries establish vector alignment.
struct __declspec(align(8)) CGridVector {
    float X, Y;
    CGridVector() {}
    CGridVector(float x, float y) : X(x), Y(y) {}
};
struct CGridCoordinate { long X, Y; CGridVector ToVector() const; }; // 0x00C620B0
CGridVector* __fastcall ScaleGridVector(CGridVector*, const CGridVector*, float); // 0x00C46F20
CGridVector* __fastcall AddGridVector(CGridVector*, const CGridVector*, const CGridVector*); // 0x00C46E70
struct CGridCornerAccumulator {
    void* VTable;
    void* Vectors;
    float Step;
    void InternVector(const CGridVector*, int); // 0x00C465F0
    void AddGridCorners(const CGridCoordinate*);
};
void CGridCornerAccumulator::AddGridCorners(const CGridCoordinate* coordinate)
{
    CGridVector base;
    {
        CGridVector converted = coordinate->ToVector();
        CGridVector value = converted;
        ScaleGridVector(&base, &value, Step);
    }
    CGridVector north;
    {
        CGridVector value(0, Step);
        AddGridVector(&north, &base, &value);
    }
    CGridVector east;
    {
        CGridVector value(Step, 0);
        AddGridVector(&east, &base, &value);
    }
    CGridVector northeast;
    {
        CGridVector value(Step, Step);
        AddGridVector(&northeast, &base, &value);
    }
    InternVector(&base, 1);
    InternVector(&north, 0);
    InternVector(&east, 0);
    InternVector(&northeast, 0);
}

static CGridVector seen[4];
static int marks[4], callCount, convertCount, scaleCount, addCount, bad;
static CGridCornerAccumulator* active;
static int mutateStep;
CGridVector CGridCoordinate::ToVector() const
{
    ++convertCount;
    return CGridVector((float)X, (float)Y);
}
CGridVector* __fastcall ScaleGridVector(CGridVector* out, const CGridVector* in, float scale)
{
    ++scaleCount;
    out->X = in->X * scale;
    out->Y = in->Y * scale;
    if (mutateStep) active->Step = 3;
    return out;
}
CGridVector* __fastcall AddGridVector(CGridVector* out, const CGridVector* left, const CGridVector* right)
{
    out->X = left->X + right->X;
    out->Y = left->Y + right->Y;
    ++addCount;
    if (mutateStep) active->Step += 1;
    return out;
}
void CGridCornerAccumulator::InternVector(const CGridVector* value, int marked)
{
    if (this != active || callCount >= 4) { bad = 1; return; }
    seen[callCount] = *value;
    marks[callCount++] = marked;
}
static int run(long x, long y, float step, int mutating)
{
    CGridCoordinate coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    CGridCornerAccumulator accumulator;
    accumulator.VTable = 0;
    accumulator.Vectors = 0;
    accumulator.Step = step;
    active = &accumulator;
    mutateStep = mutating;
    callCount = convertCount = scaleCount = addCount = bad = 0;
    accumulator.AddGridCorners(&coordinate);
    float bx = (float)x * step, by = (float)y * step;
    float northStep = mutating ? 3.0f : step;
    float eastStep = mutating ? 4.0f : step;
    float diagonalStep = mutating ? 5.0f : step;
    if (bad || callCount != 4 || convertCount != 1 || scaleCount != 1 || addCount != 3) return 0;
    if (seen[0].X != bx || seen[0].Y != by || marks[0] != 1) return 0;
    if (seen[1].X != bx || seen[1].Y != by + northStep || marks[1] != 0) return 0;
    if (seen[2].X != bx + eastStep || seen[2].Y != by || marks[2] != 0) return 0;
    if (seen[3].X != bx + diagonalStep || seen[3].Y != by + diagonalStep || marks[3] != 0) return 0;
    if (coordinate.X != x || coordinate.Y != y) return 0;
    return 1;
}
int main()
{
    if (!run(2, 3, 2, 0) || !run(-3, 7, 0.5f, 0) || !run(0, 0, -2, 0) ||
        !run(8, -2, 0, 0) || !run(2, -3, 2, 1)) {
        printf("00c466f0_TEST FAIL\n"); return 1;
    }
    printf("00c466f0_TEST PASS\n");
    return 0;
}