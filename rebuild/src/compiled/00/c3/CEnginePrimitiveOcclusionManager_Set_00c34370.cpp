// CEnginePrimitiveOcclusionManager::Set(const SOcclusionSetup* setup, const SOcclusionFade* fade) -- copy a setup record: the mode word always, the head record and both ranges when the mode is non-zero (else only the fallback value), then the fade record
// Leaf: ?Set@CEnginePrimitiveOcclusionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34370, length 160 bytes (146 + int3 padding).
// Callees: the 8-byte range's SetFirst @ 0x00C1BD60 (`[ecx] = arg`) and SetSecond @ 0x00C1BD70
// (`[ecx+4] = arg`), each called twice, out of line.
// Identity: the manifest label "CScriptModuleBase::SetCoordAxis" is propagated and wrong -- the only
// caller is the 0x00C21BA0 CScriptModuleBase constructor's sibling initialiser at 0x00C21CBC, which calls it
// on the 0x40-byte member at +0x38 whose constructor is 0x00C34320 (manifest
// "CEnginePrimitiveOcclusionManager", kept here as the class name; the layout below follows that ctor:
// head record +0 built by 0x00C20390, mode +0x10, range (1.0, 10.0) +0x14, range (0, 0) +0x1c, +0x24, +0x28,
// four 1.0f floats +0x2c..+0x38, two shorts +0x3c/+0x3e). Member names are descriptive.
// Shape: plain record assignments (implicit operator=); `if ((m_mode = setup->mode) != 0)` tests the loaded
// register (`test eax,eax`) -- a separate `if (setup->mode)` re-reads memory with `cmp [esi],0`.
struct SOcclusionHead {                 // 16 bytes (0x00C20390 zeroes three dwords and a byte)
    long a, b, c;
    bool flag;
};
struct SRange {
    float first, second;
    void SetFirst(float v);             // 0x00C1BD60
    void SetSecond(float v);            // 0x00C1BD70
};
struct SOcclusionFade {                 // 20 bytes
    float f0, f1, f2, f3;
    short s0, s1;
};
struct SOcclusionSetup {
    long           mode;                // +0x00
    SOcclusionHead head;                // +0x04
    float          rangeFirst;          // +0x14
    float          rangeSecond;         // +0x18
    long           fallback;            // +0x1c
    float          secondRangeSecond;   // +0x20
    long           extra;               // +0x24
};

class CEnginePrimitiveOcclusionManager { public:
    SOcclusionHead m_head;              // +0x00
    long           m_mode;              // +0x10
    SRange         m_range;             // +0x14
    SRange         m_secondRange;       // +0x1c
    long           m_extra;             // +0x24
    long           m_fallback;          // +0x28
    SOcclusionFade m_fade;              // +0x2c
    void Set(const SOcclusionSetup* setup, const SOcclusionFade* fade);
};

void CEnginePrimitiveOcclusionManager::Set(const SOcclusionSetup* setup, const SOcclusionFade* fade)
{
    if ((m_mode = setup->mode) != 0) {
        m_head = setup->head;
        m_range.SetFirst(setup->rangeFirst);
        m_range.SetSecond(setup->rangeSecond);
        m_secondRange.SetFirst(0);
        m_secondRange.SetSecond(setup->secondRangeSecond);
        m_extra = setup->extra;
    } else {
        m_fallback = setup->fallback;
    }
    m_fade = *fade;
}