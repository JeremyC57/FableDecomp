// CEnginePrimitiveManagerRepeatedStaticMeshes::FrameStart() -- point the four current-buffer pointers (+0x80..+0x8c) at the shadow buffer pairs (+0x60..+0x78) when the render settings' mode at +0x334 is 2, else at the normal pairs (+0x40..+0x58); then the common frame start
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99B50, length 96 bytes.
// Callees: the common frame start @ 0x00B99860 (a tail jump from both arms; the manifest label "engine_copy_global_transform_to_array" is propagated). Globals: the
// render settings @ 0x01436E60. Shape: each arm forms the four addresses with `lea` pairs (edx/eax) and stores them in order; `this` stays in ecx throughout.
struct SRenderSettings_01436e60 {
    unsigned char _pad[0x334];
    long          m_clothMode;                     // +0x334
};
extern SRenderSettings_01436e60* g_pRenderSettings_01436e60;   // 0x01436E60

struct SMeshBuffer { void* m_pA; void* m_pB; };   // 8 bytes

struct CEnginePrimitiveManagerRepeatedStaticMeshes {
    unsigned char _pad[0x40];
    SMeshBuffer   m_normal[4];                     // +0x40 .. +0x60
    SMeshBuffer   m_shadow[4];                     // +0x60 .. +0x80
    SMeshBuffer*  m_pCurrent[4];                   // +0x80 .. +0x90
    void FrameStartCommon();                       // 0x00B99860
    void FrameStart();
};

void CEnginePrimitiveManagerRepeatedStaticMeshes::FrameStart()
{
    if (g_pRenderSettings_01436e60->m_clothMode == 2) {
        m_pCurrent[0] = &m_shadow[0];
        m_pCurrent[1] = &m_shadow[1];
        m_pCurrent[2] = &m_shadow[2];
        m_pCurrent[3] = &m_shadow[3];
    } else {
        m_pCurrent[0] = &m_normal[0];
        m_pCurrent[1] = &m_normal[1];
        m_pCurrent[2] = &m_normal[2];
        m_pCurrent[3] = &m_normal[3];
    }
    FrameStartCommon();
}