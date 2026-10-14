// CRenderSetting_00c445c0::SetValue(float value) -- store the float at +0xfc when it differs from the current one and mark the setting dirty (bit 9 of the flag word at +0x14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C445C0, length 36 bytes.
// Callees: none. Shape: `fucompp ; fnstsw ax ; test ah,44h ; jnp` is the float `!=` (unordered counts as different), the float is copied as a dword through eax,
// the flag set with `or dword ptr,200h`. The chunk label "CEngine::Enable2DSceneRendering" is propagated (the 0x00C4xxxx module; the landed null-object
// helpers @ 0x00C34050 / 0x00C35F20 call this body); the class is named by address.
struct CRenderSetting_00c445c0 {
    unsigned char _pad[0x14];
    unsigned long m_flags;                         // +0x14: bit 9 = value changed
    unsigned char _pad18[0xfc - 0x18];
    float         m_value;                         // +0xfc
    void SetValue(float value);
};

void CRenderSetting_00c445c0::SetValue(float value)
{
    if (m_value != value) {
        m_value = value;
        m_flags |= 0x200;
    }
}