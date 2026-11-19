// Behaviour test for CMatrix4x4::AlignedPreScale (0x00A9D480), a model of the row over the shared kernel header:
// rows 0..2 of the 3x3 part are scaled per row by (X, Y, Z) (row r times scale[r]); E14/E24/E34 and row 4 are untouched; the SSE kernel and the
// x87 path agree on dyadic inputs (exact products). g++ builds it with -DFABLE_PORTABLE (the kernel's C++ expansion).
#include <stdio.h>
#include "engine/CMatrix4x4AlignedSSE.h"
#if defined(_MSC_VER)
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
struct C3DVector { float X, Y, Z; };
struct ALIGN16 C4DVector { float X, Y, Z, W; };
struct ALIGN16 CMatrix4x4 {
    float Data[4][4];
    void AlignedPreScale(const C3DVector& scale);
};
static bool g_SSEAvailable;
void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        C4DVector vect;
        vect.X = scale.X; vect.Y = scale.Y; vect.Z = scale.Z; vect.W = 0.0f;
        float* dest = &Data[0][0];
        C3DVector right = { Data[0][3], Data[1][3], Data[2][3] };
        GF_SSE_MATRIX_PRESCALE(dest, vect);
        Data[0][3] = right.X; Data[1][3] = right.Y; Data[2][3] = right.Z;
        return;
    }
    CMatrix4x4& m = *this;
    m.Data[0][0] *= scale.X; m.Data[0][1] *= scale.X; m.Data[0][2] *= scale.X;
    m.Data[1][0] *= scale.Y; m.Data[1][1] *= scale.Y; m.Data[1][2] *= scale.Y;
    m.Data[2][0] *= scale.Z; m.Data[2][1] *= scale.Z; m.Data[2][2] *= scale.Z;
}
int main()
{
    int bad = 0;
    for (int pass = 0; pass < 2; ++pass) {
        g_SSEAvailable = pass == 0;
        CMatrix4x4 m;
        for (int i = 0; i < 16; ++i) m.Data[i / 4][i % 4] = (float)(i - 6) * 0.5f;
        C3DVector s = { 2.0f, -0.5f, 4.0f };
        m.AlignedPreScale(s);
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) {
                float in = (float)(r * 4 + c - 6) * 0.5f;
                float want = (r < 3 && c < 3) ? in * (&s.X)[r] : in;
                if (m.Data[r][c] != want) { printf("pass %d [%d][%d] %g != %g\n", pass, r, c, m.Data[r][c], want); ++bad; }
            }
    }
    printf(bad ? "00a9d480_TEST FAIL\n" : "00a9d480_TEST PASS\n");
    return bad ? 1 : 0;
}