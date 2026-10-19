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