#include <cstdio>
struct C3DVector { float X, Y, Z; };
static int g_commits = 0;
struct CSoundReceiverLH {
    virtual void v00() { }
    C3DVector m_position, m_forward, m_up; void* m_pOwner;
    void Commit() { ++g_commits; }
    virtual void vfunc_1(const C3DVector& position) { m_position = position; Commit(); }
};
int main()
{
    CSoundReceiverLH r; C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    r.vfunc_1(p);
    if (g_commits != 1 || r.m_position.X != 1.0f || r.m_position.Y != 2.0f || r.m_position.Z != 3.0f) { std::printf("00aad060_TEST FAIL\n"); return 1; }
    std::printf("00aad060_TEST PASS\n");
    return 0;
}