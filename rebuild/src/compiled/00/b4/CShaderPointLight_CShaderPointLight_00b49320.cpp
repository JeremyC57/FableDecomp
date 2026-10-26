// CShaderPointLight::CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity, const CShaderLightAttenuation& attenuation) -- constructor: CBase, then position, colour, intensity and attenuation (the direction is left unset)
// Leaf: ??0CShaderPointLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B49320, length 106 bytes.
// Label correction: the catalog calls this a CShaderDirectionalLight ctor; the vtable it stores (0x012A2114)
// is CShaderPointLight (rtti-vtables.tsv), and the argument list matches its PDB layout.
// Callees: CBase::CBase() @ 0x0099A2F0.
// Shape: initialiser list in declaration order; C3DVector has the engine's memberwise user copy ctor
// (three direct dword stores), CRGBFloatColour and CShaderLightAttenuation copy implicitly (destination
// re-based with `lea ecx,[esi+N]`). `ret 10h`.
// Layout: PDB CShaderPointLight (0x40 bytes): Direction +4, Position +0x10, Colour +0x1c, Intensity +0x2c,
// Attenuation +0x30 (Attenuation0/1/2, AttenuationBase).
struct CBase {
    virtual void v00();
    CBase();                                     // 0x0099A2F0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CRGBFloatColour { public:
    float R, G, B, A;
};

class CShaderLightAttenuation { public:
    float Attenuation0;                          // +0x00
    float Attenuation1;                          // +0x04
    float Attenuation2;                          // +0x08
    float AttenuationBase;                       // +0x0C
};

struct CShaderPointLight : CBase {               // vtable 0x012A2114
    C3DVector               Direction;           // +0x04
    C3DVector               Position;            // +0x10
    CRGBFloatColour         Colour;              // +0x1c
    float                   Intensity;           // +0x2c
    CShaderLightAttenuation Attenuation;         // +0x30
    virtual void v00();
    CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity,
                      const CShaderLightAttenuation& attenuation);
};

CShaderPointLight::CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity,
                                     const CShaderLightAttenuation& attenuation)
    : Position(position), Colour(colour), Intensity(intensity), Attenuation(attenuation)
{
}