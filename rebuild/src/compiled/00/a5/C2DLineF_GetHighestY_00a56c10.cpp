// C2DLineF::GetHighestY() const -- the Max Y of the two end points
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56C10, length 39/48 bytes. No callees.
// Layout from the PDB (C2DLineF: Point1 +0 {X,Y}, Point2 +8 {X,Y}). The two floats are widened to
// double temporaries on the stack (right argument first, kept on the FPU stack with `fst`) and a
// Dinkumware-style const-reference Max (Max: `left < right ? right : left`, Min:
// `right < left ? right : left`) selects one by address (`lea eax` either way, `fld qword [eax]`).
// The label of the twin bodies in this module is swapped in the manifest; the compare direction
// (which temporary is loaded with fst and which lea is the fall-through) decides which is which.
template <class T>
inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

template <class T>
inline const T& Min(const T& left, const T& right)
{
    return right < left ? right : left;
}

struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
};

struct C2DLineF {
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    float GetHighestY() const;
};

float C2DLineF::GetHighestY() const
{
    return (float)Max<double>(Point1.Y, Point2.Y);
}