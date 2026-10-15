// CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth(float width, float fade, const CRGBColour& colour, unsigned char a, unsigned char b, unsigned char c) -- the smooth mesh-outline effect: the outline base's fields (ref 1, no owner, widths, colour, three option bytes) under the smooth vtable
// Leaf: ??0CEngineMeshEffectOutlineSmooth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3000, length 103 bytes (112 with padding). No calls.
// Label: the manifest's "CEnginePrimitive2DText::CEnginePrimitive2DText" is propagated; the two vtable stores name the class
// (rtti-vtables.tsv): 0x01239848 = CEngineMeshEffectOutlineBase (the inlined base constructor), 0x01239854 =
// CEngineMeshEffectOutlineSmooth (stored last).
// Shape: six stack arguments (`ret 18h`); the base constructor stores its vtable, +4 = 1, +8 = 0, the two dword arguments at
// +0xC/+0x10 (scheduled first), copies the CRGBColour reference into +0x14 and the three byte arguments into +0x18..+0x1A.
// The colour copy loads all four bytes before storing any (one spilled into the dead first-argument slot): its copy
// constructor forwards the four channels to an inline Set(r, g, b, a), whose right-to-left argument evaluation fixes the
// load order A, B, G, R. Member names past the PDB-less layout are descriptive.
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CEngineMeshEffectOutlineBase {              // vtable 0x01239848
    long          m_refCount;                      // +0x04
    void*         m_pOwner;                        // +0x08
    float         m_width;                         // +0x0C
    float         m_fade;                          // +0x10
    CRGBColour    m_colour;                        // +0x14
    unsigned char m_optionA;                       // +0x18
    unsigned char m_optionB;                       // +0x19
    unsigned char m_optionC;                       // +0x1A
    virtual void v00();
    CEngineMeshEffectOutlineBase(float width, float fade, const CRGBColour& colour, unsigned char a, unsigned char b,
                                 unsigned char c)
        : m_refCount(1), m_pOwner(0), m_width(width), m_fade(fade), m_colour(colour), m_optionA(a), m_optionB(b),
          m_optionC(c) { }
};

struct CEngineMeshEffectOutlineSmooth : CEngineMeshEffectOutlineBase {   // vtable 0x01239854
    virtual void v00();
    CEngineMeshEffectOutlineSmooth(float width, float fade, const CRGBColour& colour, unsigned char a, unsigned char b,
                                   unsigned char c);
};

CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth(float width, float fade, const CRGBColour& colour,
                                                               unsigned char a, unsigned char b, unsigned char c)
    : CEngineMeshEffectOutlineBase(width, fade, colour, a, b, c)
{
}