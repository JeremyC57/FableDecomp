// Grade: DIFFER(117v117) -- constant-register allocation only: retail keeps 0.5f in eax and loads 0.8f into edx, then reuses edx for the zero; the lane puts 0.8f in eax, hoists xor edx,edx to the top and reloads eax with 0.5f
// CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow() -- reset the shadow clip-volume tuning block to its "never shadow" defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BBC0, length 117 bytes. No callees.
// Layout: the stores land at +0x170..+0x1c1, inside the PDB's opaque SceneData block (engine/CEngineShadowRenderer.h
// keeps SceneData as `unsigned char[0x240]` and its CShadowClipVolume sub-layout does not line up with these offsets in
// the retail build), so the touched words are modelled locally with descriptive names.
// Shape: VC7.1 loads 0.5f / 0.8f into eax / edx once and shares them; the 0.0f words reuse the integer zero register
// (xor edx,edx) and the two flag bytes take its low byte. Stores follow the source order.
struct CEngineShadowRenderer {
    unsigned char _pad[0x170];
    float         m_casterPlane[4];       // +0x170
    unsigned char _pad180[0x18];
    float         m_halfScale[3];         // +0x198
    float         m_innerScale[3];        // +0x1a4
    float         m_depthRange;           // +0x1b0
    float         m_minimum;              // +0x1b4
    float         m_offset;               // +0x1b8
    float         m_weight;               // +0x1bc
    bool          m_clipCasters;          // +0x1c0
    bool          m_clipRecievers;        // +0x1c1
    void SetShadowClipVolumesToNeverShadow();
};

void CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow()
{
    m_innerScale[0] = 0.8f;
    m_innerScale[1] = 0.8f;
    m_innerScale[2] = 0.8f;
    m_halfScale[0] = 0.5f;
    m_halfScale[1] = 0.5f;
    m_halfScale[2] = 0.5f;
    m_depthRange = 6.0f;
    m_offset = 0.0f;
    m_weight = 1.0f;
    m_minimum = 0.0f;
    m_casterPlane[0] = 0.5f;
    m_casterPlane[1] = 0.5f;
    m_casterPlane[2] = 0.5f;
    m_casterPlane[3] = 0.5f;
    m_clipCasters = false;
    m_clipRecievers = false;
}
