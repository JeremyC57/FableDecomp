// Grade: DIFFER(106v106) -- scheduling only: retail loads 1.0f into edx right before the first 1.0f store (after the +0x10 byte store), the lane hoists `mov edx,3F800000h` to the top; every store and its order match
// CAnimatedMeshCreateParams::CAnimatedMeshCreateParams() -- default constructor of the 0x5c-byte parameter block that CEngineSubPrimitiveAnimatedMesh::Create builds on its stack: no mesh (-1), unit scales, cleared flags and counts, a 20.0f range, one instance, visible
// Leaf: ??0CAnimatedMeshCreateParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A790, length 106 bytes. No callees.
// Catalogued as CEngineWaterBackgroundSubPatch::CEngineWaterBackgroundSubPatch (a propagated label): it stores no vtable,
// takes no argument (`ret`) and its two callers (0x00AABD77, 0x00AABF9A) are CEngineSubPrimitiveAnimatedMesh::Create
// @ 0x00AABD50 constructing a stack local at [esp+24h]. The real class name is not recoverable from the body; the
// members are named by their defaults.
// Shape: body assignments in retail store order (an initialiser list stores +0x08/+0x0c before the +0x10 byte); the integer zero lives in ecx (its low byte / word for the byte and
// short members) and 1.0f in edx; 20.0f and 1 are immediates; `mov eax,ecx` first returns `this`.
struct CAnimatedMeshCreateParams {
    long           m_meshIndex;                    // +0x00 (-1: none)
    void*          m_pOwner;                       // +0x04
    float          m_scaleX;                       // +0x08
    float          m_scaleY;                       // +0x0c
    bool           m_flag10;                       // +0x10
    long           m_word14;                       // +0x14
    long           m_word18;                       // +0x18
    long           m_word1c;                       // +0x1c
    long           m_word20;                       // +0x20
    bool           m_flag24;                       // +0x24
    short          m_short26;                      // +0x26
    short          m_short28;                      // +0x28
    short          m_short2a;                      // +0x2a
    short          m_short2c;                      // +0x2c
    float          m_alpha;                        // +0x30
    float          m_brightness;                   // +0x34
    long           m_word38;                       // +0x38
    float          m_speed;                        // +0x3c
    float          m_range;                        // +0x40
    long           m_instanceCount;                // +0x44
    long           m_word48;                       // +0x48
    short          m_short4c;                      // +0x4c
    short          m_short4e;                      // +0x4e
    long           m_word50;                       // +0x50
    bool           m_visible;                      // +0x54
    float          m_lodScale;                     // +0x58
    CAnimatedMeshCreateParams();
};

CAnimatedMeshCreateParams::CAnimatedMeshCreateParams()
{
    m_meshIndex = -1;
    m_pOwner = 0;
    m_flag10 = false;
    m_scaleX = 1.0f;
    m_scaleY = 1.0f;
    m_word14 = 0;
    m_word18 = 0;
    m_word1c = 0;
    m_word20 = 0;
    m_flag24 = false;
    m_short26 = 0;
    m_short28 = 0;
    m_short2a = 0;
    m_short2c = 0;
    m_alpha = 1.0f;
    m_brightness = 1.0f;
    m_word38 = 0;
    m_speed = 1.0f;
    m_range = 20.0f;
    m_instanceCount = 1;
    m_word48 = 0;
    m_short4c = 0;
    m_short4e = 0;
    m_word50 = 0;
    m_visible = true;
    m_lodScale = 1.0f;
}
