// Behaviour test for 0x00ADEBC0 C3DBoundingBox::CalculateMatrix (model copy).
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

float g_epsilon_0129ba3c = 1e-4f;

class C3DBoundingBox { public:
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

int main()
{
    C3DBoundingBox box;
    box.MinCoord = C3DVector(-1.0f, 2.0f, 5.0f);
    box.MaxCoord = C3DVector(3.0f, 2.0f, 9.0f);   // flat in Y: clamped to the epsilon
    CMatrix3x4 m;
    box.CalculateMatrix(m);
    float eps = 1e-4f;
    if (m.E11 != 2.0f || m.E22 != eps || m.E33 != 2.0f || m.E41 != 1.0f || m.E42 != 2.0f || m.E43 != 7.0f ||
        m.E12 != 0.0f || m.E13 != 0.0f || m.E21 != 0.0f || m.E23 != 0.0f || m.E31 != 0.0f || m.E32 != 0.0f) {
        std::printf("00adebc0_TEST FAIL\n"); return 1;
    }
    std::printf("00adebc0_TEST PASS\n");
    return 0;
}