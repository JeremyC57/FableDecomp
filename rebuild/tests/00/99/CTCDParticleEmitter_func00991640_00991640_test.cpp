#include <cstdio>
#include <cstring>
// Behaviour of 0x00991640 (chunk 901 piece): every component is multiplied by the +0x10 scale
struct C3DVector { float X; float Y; float Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CScaledObject_Retail {
    unsigned char _pad_0x00[0x10];
    float m_scale;
    C3DVector func_00991640(const C3DVector& v) const;
};
C3DVector CScaledObject_Retail::func_00991640(const C3DVector& v) const { return C3DVector(v.X * m_scale, v.Y * m_scale, v.Z * m_scale); }
int main()
{
    CScaledObject_Retail o; o.m_scale = 2.5f; C3DVector v(1.0f, -2.0f, 4.0f);
    C3DVector r = o.func_00991640(v);
    if (r.X != 2.5f || r.Y != -5.0f || r.Z != 10.0f) { std::printf("00991640_TEST FAIL\n"); return 1; }
    std::printf("00991640_TEST PASS\n"); return 0;
}