// CRenderHelper_006f9b20::Render(void* a, void* b) -- when the flag at +0x3d is set, the first pass (@ 0x006F9240, on `this` with ecx passed straight through); then the second pass (@ 0x006F9AC0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9B20, length 40 bytes.
// Callees: the two passes @ 0x006F9240 and @ 0x006F9AC0, both members on `this` taking (a, b); the manifest labels ("CManager::GetEmbeddedEventNames" and a Game_ helper)
// are propagated. Shape: the arguments are kept in ebx/edi, the flag byte is tested through al, the first call relies on ecx still holding `this`, the second
// reloads it from esi. The chunk label "CEnginePrimitiveManager2DAnimatedMeshes::RenderSubPrimitive" is propagated (the 0x006Fxxxx module is game code); the
// class is named by address.
struct CRenderHelper_006f9b20 {
    unsigned char _pad[0x3d];
    bool          m_firstPassEnabled;              // +0x3d
    void FirstPass(void* a, void* b);              // 0x006F9240
    void SecondPass(void* a, void* b);             // 0x006F9AC0
    void Render(void* a, void* b);
};

void CRenderHelper_006f9b20::Render(void* a, void* b)
{
    if (m_firstPassEnabled)
        FirstPass(a, b);
    SecondPass(a, b);
}
