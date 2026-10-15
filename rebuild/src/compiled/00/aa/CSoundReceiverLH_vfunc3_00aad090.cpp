// CSoundReceiverLH::vfunc_3(const C3DVector& up, const C3DVector& forward) -- store the listener's up (+0x1c) and forward (+0x10) vectors, then push the new orientation to the sound system (0x00AACFB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAD090, length 56 bytes (64 with padding).
// Vtable 0x0129E76C (CSoundReceiverLH, rtti-vtables.tsv) slot 3; slot 1 (0x00AAD060) sets the position.
// Callees: 0x00AACFB0 (a thiscall on `this`, no arguments; manifest label UpdateFogColour is garbage).
// Layout as 0x00AAD060: position +0x04, forward +0x10 (constructed (0, 0, 1)), up +0x1c (constructed (0, 1, 0)).
// Shape: the first argument copied into +0x1c, then the second into +0x10, each through `lea eax,[ecx+N]`
// and three dword moves (implicit bitwise vector assignment), `this` kept in ecx for the call, `ret 8`.
struct C3DVector {
    float X;                                // +0x00
    float Y;                                // +0x04
    float Z;                                // +0x08
};

struct CSoundReceiverLH {                   // vtable 0x0129E76C
    virtual void v00();
    C3DVector m_position;                   // +0x04
    C3DVector m_forward;                    // +0x10
    C3DVector m_up;                         // +0x1c
    void*     m_pOwner;                     // +0x28
    void Commit();                          // 0x00AACFB0
    virtual void vfunc_3(const C3DVector& up, const C3DVector& forward);
};

void CSoundReceiverLH::vfunc_3(const C3DVector& up, const C3DVector& forward)
{
    m_up = up;
    m_forward = forward;
    Commit();
}