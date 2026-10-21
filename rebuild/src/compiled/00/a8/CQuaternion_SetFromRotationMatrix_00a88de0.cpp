// CQuaternion::SetFromRotationMatrix(const CMatrix3x4&) -- build the quaternion from a rotation matrix (Shoemake)
// Retail 0x00A88DE0, 399 bytes, no callees (inline x87 fsqrt; pool constants 1.0/0.5/0.0 doubles, 1.0f).
// Shape: the classic Graphics Gems "matrix to quaternion". Positive trace: s = sqrt(trace + 1.0), W = s*0.5,
// s = 0.5/s, X/Y/Z from the off-diagonal differences read straight from the argument. Otherwise a local
// transposed 3x3 copy m[i][j] = E(j+1)(i+1) (VC sinks the copy into the else arm and keeps m[1][1] from the
// first compare), the nxt[3] = {1,2,0} table (it shares the frame slots of q[4], whose lifetime starts after
// j/k are read), the largest diagonal picks i, and the arm works in double: s is a double local (a float s
// gives `fld st(0) ; fld 0.0` for the zero test where retail has `fld 0.0 ; fld st(1)`). q[3] never reaches
// memory: W is computed last, straight from (m[j][k] - m[k][j]) * s.
// Layout: CMatrix3x4 is the PDB's E11..E43 union arm (12 floats, rows of 3); CQuaternion is X, Y, Z, W.
#include <math.h>

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

struct CQuaternion {
    float X, Y, Z, W;
    void SetFromRotationMatrix(const CMatrix3x4& mat);
};

void CQuaternion::SetFromRotationMatrix(const CMatrix3x4& mat)
{
    float tr = mat.E11 + mat.E22 + mat.E33;
    if (tr > 0.0) {
        float s = (float)sqrt(tr + 1.0);
        W = s * 0.5;
        s = 0.5 / s;
        X = (mat.E32 - mat.E23) * s;
        Y = (mat.E13 - mat.E31) * s;
        Z = (mat.E21 - mat.E12) * s;
    } else {
        float m[3][3] = {
            { mat.E11, mat.E21, mat.E31 },
            { mat.E12, mat.E22, mat.E32 },
            { mat.E13, mat.E23, mat.E33 } };
        float q[4];
        int nxt[3] = { 1, 2, 0 };
        int i = 0;
        if (m[1][1] > m[0][0]) i = 1;
        if (m[2][2] > m[i][i]) i = 2;
        int j = nxt[i];
        int k = nxt[j];
        double s = sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5;
        if (s != 0.0) s = 0.5 / s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;
        X = q[0];
        Y = q[1];
        Z = q[2];
        W = (m[j][k] - m[k][j]) * s;
    }
}