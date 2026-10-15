#include <cstdio>
struct C3DVector { float x, y, z; };
struct CSoundManagerLH {
    virtual void v00() { }
    C3DVector m_vector;
    virtual void vfunc_52(const C3DVector& v) { m_vector = v; }
};
int main()
{
    CSoundManagerLH m; C3DVector v; v.x = 1.5f; v.y = -2.0f; v.z = 3.25f;
    m.vfunc_52(v);
    if (m.m_vector.x != 1.5f || m.m_vector.y != -2.0f || m.m_vector.z != 3.25f) { std::printf("00a41800_TEST FAIL\n"); return 1; }
    std::printf("00a41800_TEST PASS\n");
    return 0;
}