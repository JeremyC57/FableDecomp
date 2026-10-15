// CEnginePrimitive2DText::CEnginePrimitive2DText(const CEnginePrimitive2DText& other) -- copy constructor of the 2D text primitive descriptor (0x2C bytes)
// Leaf: ??0CEnginePrimitive2DText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655048, length 106 bytes; size-optimised TU.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (the text at +0x24).
// Shape: the 12-byte 2D header {type, interpolation set, three flag bytes} is a POD record copied with three `movsd`
// (the /Os record copy), the position/user word/layer/depth dwords go through eax one by one, the colour bytes are all
// loaded before any is stored (one parked in the dead argument slot: the colour's copy constructor forwards its four
// channels to an inline Set), the text through its copy constructor and the font pointer last. Same layout as the
// 0x00543910 constructor of this chunk. The colour copy constructor and Set must be `__forceinline` (the /Os TU calls them
// out of line otherwise).
#pragma optimize("s", on)

struct C2DVector {
    float X, Y;
};

struct CRGBColour {
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
};

struct CFontBank;

struct SEnginePrimitive2DHeader {                  // 12 bytes, plain record
    long          m_type;                          // +0x00
    long          m_interpolation;                 // +0x04 EEngineInterpolationSet
    bool          m_flag8;                         // +0x08
    bool          m_flag9;                         // +0x09
    bool          m_flagA;                         // +0x0A
};

struct CEnginePrimitive2DText {
    SEnginePrimitive2DHeader m_header;             // +0x00
    C2DVector                m_position;           // +0x0C
    unsigned long            m_userData;           // +0x14
    long                     m_layer;              // +0x18 EEngine2DLayer
    float                    m_depth;              // +0x1C
    CRGBColour               m_colour;             // +0x20
    CWideString              m_text;               // +0x24
    CFontBank*               m_pFont;              // +0x28
    CEnginePrimitive2DText(const CEnginePrimitive2DText& other);
};

CEnginePrimitive2DText::CEnginePrimitive2DText(const CEnginePrimitive2DText& other)
    : m_header(other.m_header), m_position(other.m_position), m_userData(other.m_userData), m_layer(other.m_layer),
      m_depth(other.m_depth), m_colour(other.m_colour), m_text(other.m_text), m_pFont(other.m_pFont)
{
}