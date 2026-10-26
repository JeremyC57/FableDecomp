#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CGraphicFrameData::GetTextureQuadUVs() const -- the texture coordinates of the frame's four corners: the 1/32767 fixed-point top-left (TLX, TLY) and that corner plus the frame's pixel size divided by the texture's mip-0 dimensions, returned by value as four U then four V values (TL, TR, BR, BL)
// Leaf: GetTextureQuadUVs
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC810, 191 bytes, `ret 4` (hidden result pointer).
// Label correction: the catalog's "CEngineInternalPrimitive2DStaticMesh::GetObjectTransform" is propagated; `this` is the
//   landed CGraphicFrameData record (0x009FE550 model: ReferenceCount +0, Data +4, PTexture {Data +8, Info +0xc}, ushort TLX
//   +0x10, TLY +0x12, Width +0x14, Height +0x16, Pitch +0x18); the true method name is not in the PDB data, so it is named
//   descriptively.
// Callees: CTexture::GetDimensions(unsigned long mip) const 0x009F9DB0 (returns C2DExtentsI {X, Y} by hidden pointer, ret 8).
//   Float pool 0x0129C81C = 1/32767.
// Shape: chained double stores (`a = b = v` stores b first, then a) for each pair of equal corners; the 8-float result is
//   built in a named local and copied out with `rep movsd` (no NRVO); the extents is a by-value local; the texture-space
//   width/height divide by named float locals of the texture size (`fild ; fild ; fdivp`; a cast in the expression gives `fidiv`).
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI();
};
class CTexture { public:
    C2DExtentsI GetDimensions(unsigned long mip) const;    // 0x009F9DB0
};
class CCPPointerInfo;
struct CountedTexture {
    CTexture*       Data;
    CCPPointerInfo* Info;
};

struct CTextureQuadUVs {
    float U[4];                                        // TL, TR, BR, BL
    float V[4];
};

class CGraphicFrameData { public:
    unsigned long  ReferenceCount;                     // +0x00
    void*          Data;                               // +0x04
    CountedTexture PTexture;                           // +0x08
    unsigned short TLX, TLY, Width, Height, Pitch;     // +0x10 .. +0x18
    CTextureQuadUVs GetTextureQuadUVs() const;
};

CTextureQuadUVs CGraphicFrameData::GetTextureQuadUVs() const
{
    CTextureQuadUVs uvs;
    uvs.U[0] = uvs.U[3] = (float)TLX * (1.0f / 32767.0f);
    uvs.V[0] = uvs.V[1] = (float)TLY * (1.0f / 32767.0f);
    C2DExtentsI size = PTexture.Data->GetDimensions(0);
    float textureWidth = (float)size.X;
    float textureHeight = (float)size.Y;
    uvs.U[1] = uvs.U[2] = (float)TLX * (1.0f / 32767.0f) + (float)Width / textureWidth;
    uvs.V[2] = uvs.V[3] = (float)TLY * (1.0f / 32767.0f) + (float)Height / textureHeight;
    return uvs;
}

// ---- test doubles and driver ----
static unsigned long g_mip = 99;
C2DExtentsI::C2DExtentsI() { }
C2DExtentsI CTexture::GetDimensions(unsigned long mip) const { g_mip = mip; C2DExtentsI e; e.X = 256; e.Y = 128; return e; }
int main()
{
    static CTexture texture;
    CGraphicFrameData f;
    f.PTexture.Data = &texture;
    f.TLX = 32767 / 2; f.TLY = 0; f.Width = 64; f.Height = 32;
    CTextureQuadUVs uv = f.GetTextureQuadUVs();
    float u0 = (float)(32767 / 2) * (1.0f / 32767.0f);
    float u1 = u0 + 64.0f / 256.0f;
    bool ok = g_mip == 0 && uv.U[0] == u0 && uv.U[3] == u0 && uv.U[1] == u1 && uv.U[2] == u1
        && uv.V[0] == 0.0f && uv.V[1] == 0.0f && uv.V[2] == 0.25f && uv.V[3] == 0.25f;
    if (!ok) { std::printf("009fc810_TEST FAIL\n"); return 1; }
    std::printf("009fc810_TEST PASS\n");
    return 0;
}