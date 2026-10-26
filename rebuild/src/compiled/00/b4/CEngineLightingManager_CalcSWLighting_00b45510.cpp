// CEngineLightingManager::CalcSWLighting(const C3DVector& position, const C3DVector& normal) -- software lighting of one vertex by the global lights only: the diffuse factor is the clamped square of the normal's facing towards the main light, the backlight factor its facing away; the colour is diffuse * factor + ambient + backlight * back factor, each channel clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B45510, length 525 bytes (`ret 0Ch`: hidden result, position, normal). No callees.
// Floats: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout: CEngineLightingManager retail offsets (ThemeChannels is a 12-byte vector, so every colour after it sits 4 below the PDB):
//   DiffuseVector +0x30, PrimitiveDiffuseColour +0x48, PrimitiveAmbientColour +0x58, PrimitiveBacklightColour +0x68.
// Shape: MIN / MAX are the engine's argument-repeating macros (the MAX is evaluated again inside MIN's chosen branch) around an inline
//   Sqr; the colour sum is built through by-value CRGBFloatColour operators (right operand first); Clamp must be __forceinline
//   (a plain inline member stays a call).
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
inline float Sqr(float x) { return x * x; }

class C3DVector { public:
    float X, Y, Z;
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    __forceinline void Clamp()
    {
        if (R < 0.0f) R = 0.0f; else if (R > 1.0f) R = 1.0f;
        if (G < 0.0f) G = 0.0f; else if (G > 1.0f) G = 1.0f;
        if (B < 0.0f) B = 0.0f; else if (B > 1.0f) B = 1.0f;
        if (A < 0.0f) A = 0.0f; else if (A > 1.0f) A = 1.0f;
    }
};

class CEngineLightingManager { public:
    unsigned char   _pad00[0x30];
    C3DVector       DiffuseVector;                 // +0x30
    unsigned char   _pad3c[0xc];                   // +0x3c ThemeChannels (12-byte vector)
    CRGBFloatColour PrimitiveDiffuseColour;        // +0x48
    CRGBFloatColour PrimitiveAmbientColour;        // +0x58
    CRGBFloatColour PrimitiveBacklightColour;      // +0x68
    CRGBFloatColour CalcSWLighting(const C3DVector& position, const C3DVector& normal);
};

CRGBFloatColour CEngineLightingManager::CalcSWLighting(const C3DVector& position, const C3DVector& normal)
{
    float facing = -(DiffuseVector.Z * normal.Z + DiffuseVector.Y * normal.Y + DiffuseVector.X * normal.X);
    float diffuse = MIN(Sqr(MAX(facing, 0.0f)), 1.0f);
    float back = MAX(-facing, 0.0f);
    CRGBFloatColour colour = PrimitiveDiffuseColour * diffuse + PrimitiveAmbientColour + PrimitiveBacklightColour * back;
    colour.Clamp();
    return colour;
}