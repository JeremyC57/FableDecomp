// CNavQuadTree::SetUpMapStructure(const C2DVector* origin, long a, long b, long c) -- store the map origin (unaligned at +7 / +0xb), the three parameters at +0x10 / +0x14 / +0x18 and set the byte at +4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A762F0, length 48 bytes.
// Leaf: SetUpMapStructure
// Shape: no frame, the stores through eax/edx, `ret 10h`.
struct C2DVector { float X, Y; };
#pragma pack(push, 1)
struct CNavQuadTree {
    long          m_0;
    bool          m_setUp;                         // +0x04
    unsigned char _pad05[2];
    float         m_originX;                       // +0x07 (unaligned)
    float         m_originY;                       // +0x0b
    unsigned char _pad0f;
    long          m_10;                            // +0x10
    long          m_14;                            // +0x14
    long          m_18;                            // +0x18
    void SetUpMapStructure(const C2DVector* origin, long a, long b, long c);
};
#pragma pack(pop)

void CNavQuadTree::SetUpMapStructure(const C2DVector* origin, long a, long b, long c)
{
    m_originX = origin->X;
    m_originY = origin->Y;
    m_10 = a;
    m_14 = b;
    m_18 = c;
    m_setUp = true;
}
