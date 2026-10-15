// CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, float depth, const CWideString& text, CFontBank* font, const CRGBColour& colour, EEngineInterpolationSet interpolation) -- fills a 2D text primitive descriptor (type 0x27)
// Grade: DIFFER(ours v retail) 114v114 -- scheduling only: retail stores the position's X (`mov [esi+0Ch],edx`) after the +0x14 zero store and the depth load, ours one instruction pair earlier. Tried: plain C2DVector copy (both dwords), memberwise copy constructor (both dwords), C2DVector(float x, float y) init list (fixes the x87 Y), the same with body assignments.
// Leaf: ??0CEnginePrimitive2DText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543910, length 114 bytes.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (the text copy at +0x24).
// Shape: nine stack slots (`ret 24h`, the manifest's parameter list); the leading 64-bit id is never read. The shared 2D
// header (type word 0x27, interpolation set at +4, three cleared flag bytes from one `xor ecx,ecx` zero register), then
// the position (+0xC) rebuilt through a C2DVector(float x, float y) constructor (X by a dword move, Y through the x87 --
// a plain copy or a memberwise copy constructor moves both as dwords), a zero word (+0x14), layer, depth, the colour copy (+0x20: all four bytes loaded before any store,
// the inline Set(r, g, b, a) argument order), the text copy and the font bank. Names past the manifest types are descriptive.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

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

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
};

struct CFontBank;
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_DEFAULT = 0 };

struct CEnginePrimitive2DText {
    long                    m_type;                // +0x00
    EEngineInterpolationSet m_interpolation;       // +0x04
    bool                    m_flag8;               // +0x08
    bool                    m_flag9;               // +0x09
    bool                    m_flagA;               // +0x0A
    C2DVector               m_position;            // +0x0C
    unsigned long           m_userData;            // +0x14
    EEngine2DLayer          m_layer;               // +0x18
    float                   m_depth;               // +0x1C
    CRGBColour              m_colour;              // +0x20
    CWideString             m_text;                // +0x24
    CFontBank*              m_pFont;               // +0x28
    CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, float depth,
                           const CWideString& text, CFontBank* font, const CRGBColour& colour,
                           EEngineInterpolationSet interpolation);
};

CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer,
                                               float depth, const CWideString& text, CFontBank* font,
                                               const CRGBColour& colour, EEngineInterpolationSet interpolation)
    : m_type(0x27), m_interpolation(interpolation), m_flag8(false), m_flag9(false), m_flagA(false),
      m_position(position.X, position.Y), m_userData(0), m_layer(layer), m_depth(depth), m_colour(colour), m_text(text), m_pFont(font)
{
}
