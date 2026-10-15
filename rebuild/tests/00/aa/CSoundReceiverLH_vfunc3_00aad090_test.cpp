#include <cstdio>
struct C3DVector { float X, Y, Z; };
static int g_commits = 0;
struct CSoundReceiverLH {
    virtual void v00() { }
    C3DVector m_position, m_forward, m_up; void* m_pOwner;
    void Commit() { ++g_commits; }
    virtual void vfunc_3(const C3DVector& up, const C3DVector& forward) { m_up = up; m_forward = forward; Commit(); }
};
int main()
{
    CSoundReceiverLH r; C3DVector u, f; u.X = 0.0f; u.Y = 1.0f; u.Z = 0.0f; f.X = 0.0f; f.Y = 0.0f; f.Z = 1.0f;
    r.vfunc_3(u, f);
    if (g_commits != 1 || r.m_up.Y != 1.0f || r.m_up.Z != 0.0f || r.m_forward.Z != 1.0f || r.m_forward.Y != 0.0f) { std::printf("00aad090_TEST FAIL\n"); return 1; }
    std::printf("00aad090_TEST PASS\n");
    return 0;
}