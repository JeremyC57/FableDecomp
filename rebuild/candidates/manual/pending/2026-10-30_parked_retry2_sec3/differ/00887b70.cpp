// CBlendedEnvironmentTheme::Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t) const -- linear blend of two environment themes: out = this * (1 - t) + other * t for the time of day, the 19 colours and the 30 scalers, then each of the three texture sets through BlendTextureSets
// Grade: DIFFER(1736v1769) -- leaf fixed (?Blend@CBlendedEnvironmentTheme; the parked leaf matched no symbol); 609 difflines: this/other swap esi<->ebx (retail: other in ebx, this in esi, this-other spilled to the other slot) and retail's 8 extra frame bytes (0x3c v 0x34). Swapped term order and a named const& this alias did not move the registers
// Leaf: ?Blend@CBlendedEnvironmentTheme
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00887B70, length 1776 bytes, `ret 0Ch`.
// Callees: CBlendedEnvironmentTheme::BlendTextureSets(from, to, result, blend) @ 0x00887830 (the landed source's
//   signature). Float pool: 0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CBlendedEnvironmentTheme {float TimeOfDay; CRGBFloatColour Colours[19]; float Scalers[30];
//   CIDrawEnvironmentTextureSet TextureSets[3]} with retail 12-byte STLport CArrays (texture sets 0x18 bytes, at +0x1ac),
//   as the landed BlendTextureSets 0x00887830 and GetBlendedTheme 0x00888260.
// Shape: 1 - t is computed once; the colour blend is the inline CRGBFloatColour operator* / operator+ pair (the
//   other*t temporary is built first: the operator+ arguments are evaluated right to left) and the sum is copied into
//   out through an integer register; VC7.1 unrolls the 19-colour loop by eight and the 30-scaler loop by ten.
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
template <class T> struct CArray {             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                    // +0x00
    CArray<float> Blends;                      // +0x0c
};
class CBlendedEnvironmentTheme {
public:
    float                       TimeOfDay;     // +0x000
    CRGBFloatColour             Colours[19];   // +0x004
    float                       Scalers[30];   // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];   // +0x1ac
    void BlendTextureSets(const CIDrawEnvironmentTextureSet& from, const CIDrawEnvironmentTextureSet& to,
                          CIDrawEnvironmentTextureSet& result, float blend) const;   // 0x00887830
    void Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t) const;
};

void CBlendedEnvironmentTheme::Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t) const
{
    float s = 1.0f - t;
    out.TimeOfDay = TimeOfDay * s + other.TimeOfDay * t;
    long i;
    for (i = 0; i < 19; ++i)
        out.Colours[i] = Colours[i] * s + other.Colours[i] * t;
    for (i = 0; i < 30; ++i)
        out.Scalers[i] = Scalers[i] * s + other.Scalers[i] * t;
    for (i = 0; i < 3; ++i)
        BlendTextureSets(TextureSets[i], other.TextureSets[i], out.TextureSets[i], t);
}
