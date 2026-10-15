// CRangedObject::SetRange(long first, long last) -- stores the pair at +0x1d0 / +0x1d4 and raises bit 18 of the flags word at +0x1d8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C417A0, length 40 bytes.
// Leaf: SetRange
// The manifest label "CShaderRenderManager::SetProjectionTransform" (16 floats) is propagated: `ret 8` takes two
// dwords. The class and method names follow the landed caller 0x00C33F20 (SetRangeOnAcceptingObject).
// Shape: both arguments loaded first, the flag word read / or'ed / written around the second store.
struct CRangedObject {
    unsigned char _pad000[0x1d0];
    long          m_first;                      // +0x1d0
    long          m_last;                       // +0x1d4
    unsigned long m_flags;                      // +0x1d8

    void SetRange(long first, long last);
};

void CRangedObject::SetRange(long first, long last)
{
    m_first = first;
    m_last = last;
    m_flags |= 0x40000;
}