#include <cstdio>
namespace N_0099a2d0 { struct CBase { virtual void v00() {} CBase() {} }; } using N_0099a2d0::CBase;
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct SReceiverState { void* m_pSound; void* m_pChannel; float m_volume; SReceiverState() : m_pSound(0), m_pChannel(0), m_volume(1.0f) { } };
struct CSoundReceiverLH : CBase { unsigned char _pad[0x0c]; SReceiverState m_state; C3DVector m_up; void* m_pOwner; virtual void v00() {} CSoundReceiverLH(void* owner); };
CSoundReceiverLH::CSoundReceiverLH(void* owner) : CBase(), m_state(), m_up(0.0f, 1.0f, 0.0f) { m_pOwner = owner; }
int main()
{
    int owner;
    CSoundReceiverLH r(&owner);
    if (r.m_state.m_pSound != 0 || r.m_state.m_pChannel != 0 || r.m_state.m_volume != 1.0f || r.m_up.X != 0.0f || r.m_up.Y != 1.0f || r.m_up.Z != 0.0f || r.m_pOwner != &owner) { std::printf("00aad020_TEST FAIL\n"); return 1; }
    std::printf("00aad020_TEST PASS\n");
    return 0;
}