#include <cstdio>
#include <cstring>
// Behaviour of 0x00AACF90 (chunk 901 piece): slot 2 forwards its position to slot 1
class C3DVector { public: float X; float Y; float Z; };
static int g_set = 0;
struct CSoundReceiverLH {
    virtual void v00() {}
    virtual void vfunc_1(const C3DVector& position) { m_position = position; ++g_set; }
    virtual void vfunc_2(const C3DVector& position);
    C3DVector m_position; C3DVector m_forward; C3DVector m_up; void* m_pOwner;
};
void CSoundReceiverLH::vfunc_2(const C3DVector& position) { vfunc_1(position); }
int main()
{
    CSoundReceiverLH r; std::memset(&r.m_position, 0, sizeof r.m_position); C3DVector p = { 1.0f, 2.0f, 3.0f };
    r.vfunc_2(p);
    if (g_set != 1 || r.m_position.X != 1.0f || r.m_position.Y != 2.0f || r.m_position.Z != 3.0f) { std::printf("00aacf90_TEST FAIL\n"); return 1; }
    std::printf("00aacf90_TEST PASS\n"); return 0;
}