#include <cstdio>
#include <cstring>
// Behaviour of 0x00AACFA0 (chunk 901 piece): slot 4 returns a reference to the position at +0x04
struct C3DVector { float X; float Y; float Z; };
struct CSoundReceiverLH {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual const C3DVector& vfunc_4();
    C3DVector m_position; C3DVector m_forward; C3DVector m_up; void* m_pOwner;
};
const C3DVector& CSoundReceiverLH::vfunc_4() { return m_position; }
int main()
{
    CSoundReceiverLH r; r.m_position.X = 4.0f; r.m_position.Y = 5.0f; r.m_position.Z = 6.0f;
    const C3DVector& p = r.vfunc_4();
    if (&p != &r.m_position || p.Y != 5.0f) { std::printf("00aacfa0_TEST FAIL\n"); return 1; }
    std::printf("00aacfa0_TEST PASS\n"); return 0;
}