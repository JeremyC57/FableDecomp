// CShaderDirectionalLight::ConvertToGenericLight(CShaderGenericLight& light) -- express the directional light as a generic two-parameter shader light: (direction, 0) + colour, types {direction, none}
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F39D40, 72 bytes (`?ConvertToGenericLight@CShaderDirectionalLight@@UBEXAAVCShaderGenericLight@@@Z`,
// Ego_r.pdb; vtable 0x012A221C slot 1). No callees.
//
// Shape: field-by-field stores in source order (Size first), every float copied as a dword through a
// scratch register; the 0.0f W and the PARAMETER_TYPE_NONE type share one zero register (edx) with the
// float zero emitted as the same bit pattern.
// Layouts = Ego_r.pdb: CShaderDirectionalLight (60 bytes) {vptr, C3DVector Direction +4, Position +0x10,
// CRGBFloatColour Colour +0x1C, BackColour +0x2C}; CShaderGenericLight (52 bytes) {C4DVector Parameters[2]
// +0, EParameterType Types[2] +0x20, float Attenuation2 +0x28, AttenuationBase +0x2C, unsigned long Size
// +0x30}. The enum values 1 (direction) / 2 (position) / 0 (none) are named from their use.
// Leaf: ?ConvertToGenericLight@CShaderDirectionalLight

class C3DVector { public:
    float X, Y, Z;
};

class C4DVector { public:
    float X, Y, Z, W;
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

enum EParameterType {
    PARAMETER_TYPE_NONE = 0,
    PARAMETER_TYPE_DIRECTION = 1,
    PARAMETER_TYPE_POSITION = 2
};

class CShaderGenericLight { public:
    C4DVector      Parameters[2];                // +0x00
    EParameterType Types[2];                     // +0x20
    float          Attenuation2;                 // +0x28
    float          AttenuationBase;              // +0x2C
    unsigned long  Size;                         // +0x30
};

struct CShaderDirectionalLight {
    virtual void v00();
    virtual void ConvertToGenericLight(CShaderGenericLight& light) const;   // slot 1
    C3DVector       Direction;                   // +0x04
    C3DVector       Position;                    // +0x10
    CRGBFloatColour Colour;                      // +0x1C
    CRGBFloatColour BackColour;                  // +0x2C
};

void CShaderDirectionalLight::ConvertToGenericLight(CShaderGenericLight& light) const
{
    light.Size = 2;
    light.Parameters[0].X = Direction.X;
    light.Parameters[0].Y = Direction.Y;
    light.Parameters[0].Z = Direction.Z;
    light.Parameters[0].W = 0.0f;
    light.Parameters[1].X = Colour.R;
    light.Parameters[1].Y = Colour.G;
    light.Parameters[1].Z = Colour.B;
    light.Parameters[1].W = Colour.A;
    light.Types[0] = PARAMETER_TYPE_DIRECTION;
    light.Types[1] = PARAMETER_TYPE_NONE;
}