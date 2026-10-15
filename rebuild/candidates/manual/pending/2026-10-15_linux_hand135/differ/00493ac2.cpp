// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, unsigned long textureId, unsigned long frame, EEnginePrimitiveBlendMode blendMode, EEngineInterpolationSet interpolation, float width, float height, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue, unsigned long flags, const C2DVector& uv0, const C2DVector& uv1) -- fills a 2D sprite primitive descriptor (type 0x22)
// Grade: DIFFER(ours v retail) 153v153 -- scheduling only: retail forms `lea ecx,[esi+0Ch]` for the bounds after the type/+4 stores (ours right after `mov esi,ecx`), and puts `and dword ptr [esi+40h],0` before `mov eax,[ebp+40h]` (ours after). Tried: header as a non-polymorphic base (also mis-ordered the colour), flattened header members + body colour R,G,B,A (fixed the colour), trailing members as body assignments.
// Leaf: ??0CEnginePrimitive2DSprite
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493AC2, length 153 bytes; size-optimised, frame-pointer TU.
// Callees: the 2D bounds constructor @ 0x00493AC2's 0x004371A0 (manifest "CRightHandedSet"; copies two 8-byte points into
// 16 bytes), called with the position for both corners. Parameter types are the manifest's (17 slots, `ret 44h`); the
// leading 64-bit id is never read.
// Shape: an inline 2D-primitive base constructor stores the type word (0x22), the interpolation set at +4 and clears the
// three flag bytes +8..+0xA (the same base the 0x00550A70 descriptor uses with type 0x25); then the members in declaration
// order: bounds (+0xC), layer, texture, frame, width, height, blend mode, the CRGBColour (+0x34, stored R, G, B, A from
// (alpha, red, green, blue) parameters), a visible flag byte = 1 (+0x38), flags (+0x3C), a zero word (+0x40, `and
// dword ptr [..],0` = /Os), and the two UV points copied word by word. `push dword ptr [ebp+N]` with a frame pointer and no
// spills = `#pragma optimize("y", off)` + `optimize("s", on)`. The names past the manifest types are descriptive.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct C2DVector {
    float X, Y;
};

struct C2DBounds {                                 // 16 bytes: two corners
    C2DVector Min, Max;
    C2DBounds(const C2DVector& a, const C2DVector& b);   // 0x004371A0
};

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };
enum EEnginePrimitiveBlendMode { ENGINE_BLEND_DEFAULT = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_DEFAULT = 0 };

struct CEnginePrimitive2DSprite {
    long                      m_type;              // +0x00
    EEngineInterpolationSet   m_interpolation;     // +0x04
    bool                      m_flag8;             // +0x08
    bool                      m_flag9;             // +0x09
    bool                      m_flagA;             // +0x0A
    C2DBounds                 m_bounds;            // +0x0C
    EEngine2DLayer            m_layer;             // +0x1C
    unsigned long             m_textureId;         // +0x20
    unsigned long             m_frame;             // +0x24
    float                     m_width;             // +0x28
    float                     m_height;            // +0x2C
    EEnginePrimitiveBlendMode m_blendMode;         // +0x30
    CRGBColour                m_colour;            // +0x34
    bool                      m_visible;           // +0x38
    unsigned long             m_flags;             // +0x3C
    unsigned long             m_userData;          // +0x40
    C2DVector                 m_uv0;               // +0x44
    C2DVector                 m_uv1;               // +0x4C
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, unsigned long textureId,
                             unsigned long frame, EEnginePrimitiveBlendMode blendMode, EEngineInterpolationSet interpolation,
                             float width, float height, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long flags, const C2DVector& uv0, const C2DVector& uv1);
};

CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer,
                                                   unsigned long textureId, unsigned long frame,
                                                   EEnginePrimitiveBlendMode blendMode,
                                                   EEngineInterpolationSet interpolation, float width, float height,
                                                   unsigned char alpha, unsigned char red, unsigned char green,
                                                   unsigned char blue, unsigned long flags, const C2DVector& uv0,
                                                   const C2DVector& uv1)
    : m_type(0x22), m_interpolation(interpolation), m_flag8(false), m_flag9(false), m_flagA(false), m_bounds(position, position), m_layer(layer), m_textureId(textureId),
      m_frame(frame), m_width(width), m_height(height), m_blendMode(blendMode), m_colour(red, green, blue, alpha)
{
    m_userData = 0;
    m_flags = flags;
    m_visible = true;
    m_uv0 = uv0;
    m_uv1 = uv1;
}
