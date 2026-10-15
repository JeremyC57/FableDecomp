// CShaderPointLight::ConvertToGenericLight(CShaderGenericLight& light) -- express the point light as a generic shader light: (position, 0) + colour, Attenuation2 inverted, AttenuationBase copied, types {position, none}
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F39D90, 90 bytes (`?ConvertToGenericLight@CShaderPointLight@@UBEXAAVCShaderGenericLight@@@Z`,
// Ego_r.pdb; vtable 0x012A2114 slot 1). No callees; 1.0f is the pool constant at 0x0122DED8.
//
// Shape: as the directional twin (0x00F39D40), with `1.0f / Attenuation.Attenuation2` (`fld [1.0] ; fdiv
// [ecx+38h]`, the constant load hoisted to the top) stored between the colour copy and AttenuationBase.
// Layouts = Ego_r.pdb: CShaderPointLight (64 bytes) {vptr, C3DVector Direction +4, Position +0x10,
// CRGBFloatColour Colour +0x1C, float Intensity +0x2C, CShaderLightAttenuation Attenuation +0x30
// (Attenuation0/1/2, AttenuationBase)}; CShaderGenericLight (52 bytes) as in the directional twin.
// Leaf: ?ConvertToGenericLight@CShaderPointLight

struct C3DVector {
    float X, Y, Z;
};

struct C4DVector {
    float X, Y, Z, W;
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CShaderLightAttenuation {
    float Attenuation0;                          // +0x00
    float Attenuation1;                          // +0x04
    float Attenuation2;                          // +0x08
    float AttenuationBase;                       // +0x0C
};

enum EParameterType {
    PARAMETER_TYPE_NONE = 0,
    PARAMETER_TYPE_DIRECTION = 1,
    PARAMETER_TYPE_POSITION = 2
};

struct CShaderGenericLight {
    C4DVector      Parameters[2];                // +0x00
    EParameterType Types[2];                     // +0x20
    float          Attenuation2;                 // +0x28
    float          AttenuationBase;              // +0x2C
    unsigned long  Size;                         // +0x30
};

struct CShaderPointLight {
    virtual void v00();
    virtual void ConvertToGenericLight(CShaderGenericLight& light) const;   // slot 1
    C3DVector               Direction;           // +0x04
    C3DVector               Position;            // +0x10
    CRGBFloatColour         Colour;              // +0x1C
    float                   Intensity;           // +0x2C
    CShaderLightAttenuation Attenuation;         // +0x30
};

void CShaderPointLight::ConvertToGenericLight(CShaderGenericLight& light) const
{
    light.Size = 2;
    light.Parameters[0].X = Position.X;
    light.Parameters[0].Y = Position.Y;
    light.Parameters[0].Z = Position.Z;
    light.Parameters[0].W = 0.0f;
    light.Parameters[1].X = Colour.R;
    light.Parameters[1].Y = Colour.G;
    light.Parameters[1].Z = Colour.B;
    light.Parameters[1].W = Colour.A;
    light.Attenuation2 = 1.0f / Attenuation.Attenuation2;
    light.AttenuationBase = Attenuation.AttenuationBase;
    light.Types[0] = PARAMETER_TYPE_POSITION;
    light.Types[1] = PARAMETER_TYPE_NONE;
}
