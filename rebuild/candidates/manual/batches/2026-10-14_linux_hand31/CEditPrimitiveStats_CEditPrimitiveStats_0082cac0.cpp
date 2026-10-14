// CEditPrimitiveStats::CEditPrimitiveStats(const CEditPrimitiveStats&) -- member-wise copy: two words, a vector (@ 0x0082C620), a CSurface (@ 0x009F2D60), a CTexture (@ 0x009F9D00), a word, a second vector (@ 0x0082C8A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082CAC0, length 79 bytes.
// Callees: the out-of-line copy constructors of the four class members (each called with the member's address in ecx and the source member pushed).
// Shape: the compiler-generated copy constructor -- the plain words are copied in declaration order around the member calls (the +0x2c word's store is
// scheduled between the last member's `lea` and its call), `this` is returned. The PDB has no layout for CEditPrimitiveStats; the member types follow the
// callees' manifest names (CSurface 16 bytes, CTexture 8 bytes, the vectors 12 bytes).
struct CRetailVector_0082c620 {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CRetailVector_0082c620(const CRetailVector_0082c620& other);   // 0x0082C620
};

struct CSurface {
    unsigned char _body[0x10];
    CSurface(const CSurface& other);               // 0x009F2D60
};

struct CTexture {
    unsigned char _body[8];
    CTexture(const CTexture& other);               // 0x009F9D00
};

struct CRetailVector_0082c8a0 {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CRetailVector_0082c8a0(const CRetailVector_0082c8a0& other);   // 0x0082C8A0
};

struct CEditPrimitiveStats {
    long                   m_primitiveCount;       // +0x00
    long                   m_vertexCount;          // +0x04
    CRetailVector_0082c620 m_counts;               // +0x08
    CSurface               m_surface;              // +0x14
    CTexture               m_texture;              // +0x24
    long                   m_flags;                // +0x2c
    CRetailVector_0082c8a0 m_sizes;                // +0x30
    CEditPrimitiveStats(const CEditPrimitiveStats& other);
};

CEditPrimitiveStats::CEditPrimitiveStats(const CEditPrimitiveStats& other)
    : m_primitiveCount(other.m_primitiveCount), m_vertexCount(other.m_vertexCount), m_counts(other.m_counts), m_surface(other.m_surface),
      m_texture(other.m_texture), m_flags(other.m_flags), m_sizes(other.m_sizes)
{
}
