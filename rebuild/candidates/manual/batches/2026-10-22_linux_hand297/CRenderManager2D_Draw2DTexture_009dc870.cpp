// CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour, CTexture& texture, unsigned long flags) -- queue a textured screen-space box as two triangles, nudging the UVs by a hundredth of a texel
// Leaf: Draw2DTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC870, length 525 bytes (ret 0x14); PDB
// ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z.
// Callees: GFVBoxToABox 0x009E1E70 (__fastcall, box in, converted box out), CTexture::GetPixelFormat 0x009F9D80
// (CPixelFormat by hidden pointer), CTexture::GetDimensions 0x009F9DB0 (C2DExtentsI by hidden pointer, level 0) twice,
// the pixel-format info table 0x0129BA60 (36-byte rows, leading bool), the screen RHW float 0x0139652C,
// CRenderManager2D::Add2DTri 0x009DC030 (v0, v1, v2, mode, &texture) twice.
// Layout: CUVBox (Ego_r.pdb) is { float U[4]; float V[4]; } for the TL, TR, BR, BL corners.
// Shape: corners from the converted box (TL, TR, BR, BL) and the ARGB word first, then the texel offset: 0.01 for a
// format whose table row flag is set, else 0.01 / width and 0.01 / height (unsigned to float); then Z 0, RHW, and
// U/V = offset + uv (per vertex: Diffuse, X, Y first; U, V, Z, RHW after the offset -- the store order that
// reproduces retail's schedule). Blend mode: flag 2 picks 2, else flag 1 picks 3, else 1. Triangles (0, 1, 2) and (0, 2, 3).
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CUVBox { float U[4]; float V[4]; };
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) | R) << 8 | G) << 8) | B; }
};
struct CPixelFormat { long FormatIndex; };
struct C2DExtentsI { unsigned long Width, Height; };
struct CTexture {
    CPixelFormat GetPixelFormat() const;               // 0x009F9D80
    C2DExtentsI GetDimensions(unsigned long level);    // 0x009F9DB0
};
struct SPixelFormatInfo {                              // 36-byte rows
    bool          IsCompressed;
    unsigned char _pad01[0x23];
};
extern SPixelFormatInfo g_PixelFormatInfo[];          // 0x0129BA60
struct CTVertexRHWColSpecTex1 {
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
void __fastcall GFVBoxToABox(const C2DBoxF* box, C2DBoxF* out);   // 0x009E1E70
extern float g_ScreenRHW;                          // 0x0139652C

struct CRenderManager2D {
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long mode, CTexture* texture);   // 0x009DC030
    bool Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour, CTexture& texture,
                       unsigned long flags);
};

bool CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour,
                                     CTexture& texture, unsigned long flags)
{
    unsigned long argb = colour.GetARGB();
    C2DBoxF vbox;
    GFVBoxToABox(&box, &vbox);
    CTVertexRHWColSpecTex1 v[4];
    v[0].Diffuse = argb;
    v[0].X = vbox.TLX;
    v[0].Y = vbox.TLY;
    v[1].Diffuse = argb;
    v[1].X = vbox.BRX;
    v[1].Y = vbox.TLY;
    v[2].Diffuse = argb;
    v[2].X = vbox.BRX;
    v[2].Y = vbox.BRY;
    v[3].Diffuse = argb;
    v[3].X = vbox.TLX;
    v[3].Y = vbox.BRY;
    float offsetU;
    float offsetV;
    if (g_PixelFormatInfo[texture.GetPixelFormat().FormatIndex].IsCompressed) {
        offsetU = 0.01f;
        offsetV = 0.01f;
    } else {
        offsetU = 0.01f / (float)texture.GetDimensions(0).Width;
        offsetV = 0.01f / (float)texture.GetDimensions(0).Height;
    }
    v[0].U = offsetU + uvs.U[0];
    v[0].V = offsetV + uvs.V[0];
    v[0].Z = 0.0f;
    v[0].RHW = g_ScreenRHW;
    v[1].U = offsetU + uvs.U[1];
    v[1].V = offsetV + uvs.V[1];
    v[1].Z = 0.0f;
    v[1].RHW = g_ScreenRHW;
    v[2].U = offsetU + uvs.U[2];
    v[2].V = offsetV + uvs.V[2];
    v[2].Z = 0.0f;
    v[2].RHW = g_ScreenRHW;
    v[3].U = offsetU + uvs.U[3];
    v[3].V = offsetV + uvs.V[3];
    v[3].Z = 0.0f;
    v[3].RHW = g_ScreenRHW;
    long mode;
    if (flags & 2)
        mode = 2;
    else if (flags & 1)
        mode = 3;
    else
        mode = 1;
    Add2DTri(v[0], v[1], v[2], mode, &texture);
    Add2DTri(v[0], v[2], v[3], mode, &texture);
    return true;
}
