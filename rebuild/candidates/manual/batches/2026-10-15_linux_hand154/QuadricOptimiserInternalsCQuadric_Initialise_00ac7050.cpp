// QuadricOptimiserInternals::CQuadric::Initialise(const C3DVector& normal, const C3DVector& point, float weight) -- build the plane quadric of the plane through `point` with `normal`: the outer product of (a, b, c, d) with d = -normal.point, plus the weight
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7050, length 149 bytes (padded to 160), `ret 0Ch`. No callees.
// Layout: eleven doubles AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight (PDB; landed operator*= 0x00AC6EF0); C3DVector is
// three floats X, Y, Z.
// Shape: a and b stay on the FPU stack, c and d are float locals VC7.1 parks in the dead argument slots; the plane offset is
// the inline C3DVector::DotProduct (Z + Y + X order, landed 0x00C256A0) negated; every product is `term = first * second`
// (the right operand is loaded first).
struct C3DVector {
    float X, Y, Z;
    float DotProduct(const C3DVector& other) const { return Z * other.Z + Y * other.Y + X * other.X; }
};
namespace QuadricOptimiserInternals {

struct CQuadric {
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    void Initialise(const C3DVector& normal, const C3DVector& point, float weight);
};

void CQuadric::Initialise(const C3DVector& normal, const C3DVector& point, float weight)
{
    float a = normal.X;
    float b = normal.Y;
    float c = normal.Z;
    float d = -normal.DotProduct(point);
    AA = a * a;
    AB = a * b;
    AC = a * c;
    AD = a * d;
    BB = b * b;
    BC = b * c;
    BD = b * d;
    CC = c * c;
    CD = c * d;
    DD = d * d;
    Weight = weight;
}

}
