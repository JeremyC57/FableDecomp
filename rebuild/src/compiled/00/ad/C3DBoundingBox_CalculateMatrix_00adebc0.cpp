// C3DBoundingBox::CalculateMatrix(CMatrix3x4& matrix) const -- the box as a transform: half extents (clamped to a minimum) on the diagonal, the centre as the position
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADEBC0, 242 bytes (`ret 4`).
// Ego_r.pdb: ?CalculateMatrix@C3DBoundingBox@@QBEXAAVCMatrix3x4@@@Z. No callees (0.5f pool 0x0122F59C, the 1e-4f
// global 0x0129BA3C read as a variable: `mov eax,[g]` copies).
// Shape: centre = (Max + Min) * 0.5f and half = (Max - Min) * 0.5f through C3DVector value operators with a user
// ctor (intermediates on the x87 stack, the overflow spilled); each half extent below the epsilon (`test ah,5 ; jp`)
// is raised to it; the matrix gets the three scaled axes, zeros elsewhere (one `xor ecx,ecx` register) and the centre.
// Layout: PDB C3DBoundingBox MinCoord +0, MaxCoord +0xc; CMatrix3x4 E11..E43 (rows of three floats).
// Leaf: CalculateMatrix@C3DBoundingBox
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

extern float g_epsilon_0129ba3c;                   // 0x0129BA3C (1e-4f)

struct C3DBoundingBox {
    C3DVector MinCoord;                            // +0
    C3DVector MaxCoord;                            // +0xc
    void CalculateMatrix(CMatrix3x4& matrix) const;
};

void C3DBoundingBox::CalculateMatrix(CMatrix3x4& matrix) const
{
    C3DVector centre = (MaxCoord + MinCoord) * 0.5f;
    C3DVector half = (MaxCoord - MinCoord) * 0.5f;
    if (half.X < g_epsilon_0129ba3c)
        half.X = g_epsilon_0129ba3c;
    if (half.Y < g_epsilon_0129ba3c)
        half.Y = g_epsilon_0129ba3c;
    if (half.Z < g_epsilon_0129ba3c)
        half.Z = g_epsilon_0129ba3c;
    matrix.E11 = half.X;
    matrix.E12 = 0.0f;
    matrix.E13 = 0.0f;
    matrix.E21 = 0.0f;
    matrix.E22 = half.Y;
    matrix.E23 = 0.0f;
    matrix.E31 = 0.0f;
    matrix.E32 = 0.0f;
    matrix.E33 = half.Z;
    matrix.E41 = centre.X;
    matrix.E42 = centre.Y;
    matrix.E43 = centre.Z;
}