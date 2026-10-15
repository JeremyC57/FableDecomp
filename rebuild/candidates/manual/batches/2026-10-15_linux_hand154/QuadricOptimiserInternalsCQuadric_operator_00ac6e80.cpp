// QuadricOptimiserInternals::CQuadric::operator+=(const CQuadric&) -- add another quadric error matrix term by term (AA..DD and Weight)
// Leaf: ??YCQuadric
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6E80, length 103 bytes (padded to 112). No callees.
// Layout: the PDB QuadricOptimiserInternals::CQuadric is eleven doubles (AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
// 0x58 bytes), as in the landed operator*= (0x00AC6EF0).
// Shape: each term is `fld qword [other] ; fadd qword [this] ; fstp qword [this]` in declaration order (VC7.1's `m += o.m`
// loads the right operand first and adds the member in memory).
namespace QuadricOptimiserInternals {

struct CQuadric {
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    void operator+=(const CQuadric& other);
};

void CQuadric::operator+=(const CQuadric& other)
{
    AA += other.AA;
    AB += other.AB;
    AC += other.AC;
    AD += other.AD;
    BB += other.BB;
    BC += other.BC;
    BD += other.BD;
    CC += other.CC;
    CD += other.CD;
    DD += other.DD;
    Weight += other.Weight;
}

}
