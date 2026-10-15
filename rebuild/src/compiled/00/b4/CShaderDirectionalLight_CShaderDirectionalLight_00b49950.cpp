// CShaderDirectionalLight::CShaderDirectionalLight(const C3DVector& direction, const CRGBFloatColour& colour, const CRGBFloatColour& backColour) -- constructor: CBase, then direction, colour and back colour (the position is left unset)
// Leaf: ??0CShaderDirectionalLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B49950, length 112 bytes (99 + int3 padding).
// Callees: CBase::CBase() @ 0x0099A2F0.
// Vtable: 0x012A221C (CShaderDirectionalLight, rtti-vtables.tsv).
// Shape: initialiser list in declaration order; C3DVector has the engine's memberwise user copy ctor
// (three direct dword stores), the two CRGBFloatColour members copy implicitly (destination re-based with
// `lea`). `ret 0Ch`.
// Layout: PDB CShaderDirectionalLight (60 bytes): Direction +4, Position +0x10, Colour +0x1c, BackColour +0x2c
// (as the landed ConvertToGenericLight 0x00F39D40).
struct CBase {
    virtual void v00();
    CBase();                                     // 0x0099A2F0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CShaderDirectionalLight : CBase {         // vtable 0x012A221C
    C3DVector       Direction;                   // +0x04
    C3DVector       Position;                    // +0x10
    CRGBFloatColour Colour;                      // +0x1c
    CRGBFloatColour BackColour;                  // +0x2c
    virtual void v00();
    CShaderDirectionalLight(const C3DVector& direction, const CRGBFloatColour& colour, const CRGBFloatColour& backColour);
};

CShaderDirectionalLight::CShaderDirectionalLight(const C3DVector& direction, const CRGBFloatColour& colour,
                                                 const CRGBFloatColour& backColour)
    : Direction(direction), Colour(colour), BackColour(backColour)
{
}