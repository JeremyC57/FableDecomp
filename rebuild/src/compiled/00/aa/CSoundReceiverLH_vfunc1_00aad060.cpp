// CSoundReceiverLH::vfunc_1(const C3DVector& position) -- store the listener position at +0x04, then push the new orientation to the sound system (0x00AACFB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAD060, length 33 bytes (48 with padding).
// Vtable 0x0129E76C (CSoundReceiverLH, rtti-vtables.tsv) slot 1; slot 3 (0x00AAD090) sets up/forward the
// same way. Callees: 0x00AACFB0 (a thiscall on `this`, no arguments; its manifest label
// CShaderRenderManager::UpdateFogColour is propagated garbage).
// Layout: the landed constructor 0x00AAD020 leaves +0x04..+0x0c alone, stores (0, 0, 1.0) at +0x10 and
// (0, 1.0, 0) at +0x1c: position, forward and up vectors, then the owner at +0x28.
// Shape: the source read once into edx, the destination formed with `lea eax,[ecx+4]` (ecx stays `this`
// for the call), three dword moves through esi/edx: the implicit bitwise assignment of a three-float
// vector; the call cannot be a tail jump because esi is restored after it.
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
    virtual void vfunc_1(const C3DVector& position);
};

void CSoundReceiverLH::vfunc_1(const C3DVector& position)
{
    m_position = position;
    Commit();
}