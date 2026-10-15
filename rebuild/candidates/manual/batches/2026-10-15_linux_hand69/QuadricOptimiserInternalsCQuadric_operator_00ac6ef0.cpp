// QuadricOptimiserInternals::CQuadric::operator*=(double) -- scale all eleven terms of the quadric error matrix (AA..DD and Weight) by a factor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6EF0, length 91 bytes. No callees.
// Leaf: ??XCQuadric
// Identity: BSim port of ??XCQuadric@QuadricOptimiserInternals@@QAEXN@Z at similarity 1.0; the PDB layout of
// QuadricOptimiserInternals::CQuadric is eleven doubles (AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight; 0x58 bytes).
// Shape: the scale (a by-value double, `ret 8`) is loaded once and duplicated with `fld st(0)` for every member,
// the last multiply consumes it. Each `m *= s` is `fmul qword [member] ; fstp qword [member]` in declaration order.
namespace QuadricOptimiserInternals {

struct CQuadric {
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    void operator*=(double scale);
};

void CQuadric::operator*=(double scale)
{
    AA *= scale;
    AB *= scale;
    AC *= scale;
    AD *= scale;
    BB *= scale;
    BC *= scale;
    BD *= scale;
    CC *= scale;
    CD *= scale;
    DD *= scale;
    Weight *= scale;
}

}
