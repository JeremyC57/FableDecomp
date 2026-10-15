// CEnginePrimitiveLine::CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width, EEnginePrimitiveBlendMode blendMode, unsigned long flags, bool depthTest, EEngineInterpolationSet interpolation, EEnginePrimitiveBlendOp blendOp) -- fills a line primitive descriptor (type 6)
// Leaf: ??0CEnginePrimitiveLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3090, length 143 bytes (144 with padding). No calls.
// Shape: eleven stack slots (`ret 2Ch`, the manifest's parameter types); the leading 64-bit id is never read. The shared
// primitive header (type word 6, interpolation set at +4, three cleared flag bytes from one `xor cl,cl`), the two end points
// (+0xC, +0x18) copied word by word straight into [eax+N] by C3DVector's
// memberwise copy constructor (a plain POD copy forms `lea edx,[eax+0Ch]` and borrows esi), the colour (+0x24: all four bytes loaded before any is stored, one parked in the
// dead interpolation argument slot -- the colour's copy constructor forwards its channels to an inline Set(r, g, b, a)),
// then width, flags, blend mode, blend op and the depth-test byte in declaration order. Names past the manifest types are
// descriptive.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
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

enum EEnginePrimitiveBlendMode { ENGINE_BLEND_DEFAULT = 0 };
enum EEnginePrimitiveBlendOp { ENGINE_BLEND_OP_DEFAULT = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_DEFAULT = 0 };

struct CEnginePrimitiveLine {
    long                      m_type;              // +0x00
    EEngineInterpolationSet   m_interpolation;     // +0x04
    bool                      m_flag8;             // +0x08
    bool                      m_flag9;             // +0x09
    bool                      m_flagA;             // +0x0A
    C3DVector                 m_start;             // +0x0C
    C3DVector                 m_end;               // +0x18
    CRGBColour                m_colour;            // +0x24
    float                     m_width;             // +0x28
    unsigned long             m_flags;             // +0x2C
    EEnginePrimitiveBlendMode m_blendMode;         // +0x30
    EEnginePrimitiveBlendOp   m_blendOp;           // +0x34
    bool                      m_depthTest;         // +0x38
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, EEnginePrimitiveBlendMode blendMode, unsigned long flags, bool depthTest,
                         EEngineInterpolationSet interpolation, EEnginePrimitiveBlendOp blendOp);
};

CEnginePrimitiveLine::CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end,
                                           const CRGBColour& colour, float width, EEnginePrimitiveBlendMode blendMode,
                                           unsigned long flags, bool depthTest, EEngineInterpolationSet interpolation,
                                           EEnginePrimitiveBlendOp blendOp)
    : m_type(6), m_interpolation(interpolation), m_flag8(false), m_flag9(false), m_flagA(false), m_start(start),
      m_end(end), m_colour(colour), m_width(width), m_flags(flags), m_blendMode(blendMode), m_blendOp(blendOp),
      m_depthTest(depthTest)
{
}