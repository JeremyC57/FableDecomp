#include <cstdio>
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

static bool close(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
static bool check(float x, float y, float z, float w)
{
    float n = (float)sqrt(x*x + y*y + z*z + w*w); x /= n; y /= n; z /= n; w /= n;
    CMatrix3x4 r;
    r.E11 = 1 - 2*(y*y + z*z); r.E12 = 2*(x*y - w*z); r.E13 = 2*(x*z + w*y);
    r.E21 = 2*(x*y + w*z); r.E22 = 1 - 2*(x*x + z*z); r.E23 = 2*(y*z - w*x);
    r.E31 = 2*(x*z - w*y); r.E32 = 2*(y*z + w*x); r.E33 = 1 - 2*(x*x + y*y);
    r.E41 = r.E42 = r.E43 = 0;
    CQuaternion q; q.X = q.Y = q.Z = q.W = 99.0f;
    q.SetFromRotationMatrix(r);
    float sg = (q.W * w + q.X * x + q.Y * y + q.Z * z) < 0 ? -1.0f : 1.0f;
    return close(q.X, sg*x) && close(q.Y, sg*y) && close(q.Z, sg*z) && close(q.W, sg*w);
}
int main()
{
    bool ok = check(0, 0, 0, 1)          // identity, trace path
           && check(0.3f, -0.2f, 0.1f, 0.9f) // trace path
           && check(1, 0, 0, 0)          // 180 about X: i = 0
           && check(0, 1, 0, 0)          // 180 about Y: i = 1
           && check(0, 0, 1, 0)          // 180 about Z: i = 2
           && check(0.7f, 0.5f, -0.3f, -0.1f)
           && check(-0.2f, 0.4f, 0.8f, 0.05f);
    if (!ok) { std::printf("00a88de0_TEST FAIL\n"); return 1; }
    std::printf("00a88de0_TEST PASS\n");
    return 0;
}