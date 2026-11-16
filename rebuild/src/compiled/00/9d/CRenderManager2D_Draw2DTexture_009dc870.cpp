// CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour, CTexture& texture, unsigned long flags) -- queue a textured screen-space box as two triangles, nudging the UVs by a hundredth of a texel
// Leaf: ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC870, length 525 bytes (ret 0x14); PDB
// ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z.
// Callees: GFVBoxToABox 0x009E1E70 (__fastcall, box in, converted box out), CTexture::GetPixelFormat 0x009F9D80
// (CPixelFormat by hidden pointer), CTexture::GetDimensions 0x009F9DB0 (C2DExtentsI by hidden pointer, level 0) twice,
// GD3DFormats[format].Linear (0x0129BA60 = 0x0129BA40 + 0x20, CPixelFormat::IsLinear, FableWin.pdb, inline), the screen RHW float 0x0139652C,
// CRenderManager2D::Add2DTri 0x009DC030 (v0, v1, v2, mode, &texture) twice.
// Layout: CUVBox (Ego_r.pdb) is { float U[4]; float V[4]; } for the TL, TR, BR, BL corners.
// Shape: corners from the converted box (TL, TR, BR, BL) and the ARGB word first, then the texel offset: 0.01 for a
// format whose table row flag is set, else 0.01 / width and 0.01 / height (unsigned to float); then Z 0, RHW, and
// U/V = offset + uv (per vertex: Diffuse, X, Y first; U, V, Z, RHW after the offset -- the store order that
// reproduces retail's schedule). Blend mode: flag 2 picks 2, else flag 1 picks 3, else 1. Triangles (0, 1, 2) and (0, 2, 3).
// Ego_r.pdb 0x00B30450 (the UV overload, declared in engine/CRenderManager2D.h); Add2DTri is the private
//   ?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_EXTENTS_INLINE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_ISLINEAR
#define FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
#define FABLE_RETAIL_CGENERICVERTEX_ORIGINAL_RHW_PASS23   // BIGGEST_RHW_VERTEX_VALUE 0x0139652C (both PDBs, S_LDATA32 const float)
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CUVBox.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CTexture.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxF& v, C2DBoxF& a);   // 0x009E1E70

bool CRenderManager2D::Draw2DTexture(const C2DBoxF& box, const CUVBox& uvs, const CRGBColour& colour, CTexture& texture,
                                     unsigned long flags)
{
    unsigned long argb = (((((unsigned long)colour.GetA() << 8) | colour.GetR()) << 8 | colour.GetG()) << 8) | colour.GetB();
    C2DBoxF screen;
    GFVBoxToABox(box, screen);
    CTVertexRHWColSpecTex1 v[4];
    v[0].DiffuseColour = argb;
    v[0].X = screen.TLX;
    v[0].Y = screen.TLY;
    v[1].DiffuseColour = argb;
    v[1].X = screen.BRX;
    v[1].Y = screen.TLY;
    v[2].DiffuseColour = argb;
    v[2].X = screen.BRX;
    v[2].Y = screen.BRY;
    v[3].DiffuseColour = argb;
    v[3].X = screen.TLX;
    v[3].Y = screen.BRY;
    float offset_u;
    float offset_v;
    if (texture.GetPixelFormat().IsLinear()) {
        offset_u = 0.01f;
        offset_v = 0.01f;
    } else {
        offset_u = 0.01f / (float)texture.GetWidth(0);
        offset_v = 0.01f / (float)texture.GetHeight(0);
    }
    v[0].U1 = offset_u + uvs.U[0];
    v[0].V1 = offset_v + uvs.V[0];
    v[0].Z = 0.0f;
    v[0].RHW = BIGGEST_RHW_VERTEX_VALUE;
    v[1].U1 = offset_u + uvs.U[1];
    v[1].V1 = offset_v + uvs.V[1];
    v[1].Z = 0.0f;
    v[1].RHW = BIGGEST_RHW_VERTEX_VALUE;
    v[2].U1 = offset_u + uvs.U[2];
    v[2].V1 = offset_v + uvs.V[2];
    v[2].Z = 0.0f;
    v[2].RHW = BIGGEST_RHW_VERTEX_VALUE;
    v[3].U1 = offset_u + uvs.U[3];
    v[3].V1 = offset_v + uvs.V[3];
    v[3].Z = 0.0f;
    v[3].RHW = BIGGEST_RHW_VERTEX_VALUE;
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
