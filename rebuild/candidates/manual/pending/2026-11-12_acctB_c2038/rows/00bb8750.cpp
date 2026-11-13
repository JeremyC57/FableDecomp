// CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup(const CEngineInternalPrimitiveParticleDecalGroup& group, bool premultiplied_alpha, long point_light_count) -- render a group of landscape particle decals: landscape-decal vertex shader for the point-light count and its pixel shader, alpha blend / z test on, z write off, alpha test against 1, world transform = identity at the camera position; then for each decal of the group a grid of 32-byte vertices over the whole landscape cells it covers (floor / ceil of its extent; z = the decal height, normal = its RHSet up, uv from the decal centre and size), two triangles per cell split along a checkerboard diagonal, the decal colour (premultiplied by alpha when asked) as vertex-shader constant 0, PrepareForRender(0x15) and one DrawAsIndexedTris
// Leaf: ?RenderLandscapeDecalGroup@CEnginePrimitiveManagerParticleDecalGroup@@AAEXABVCEngineInternalPrimitiveParticleDecalGroup@@_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB8750, 1951 bytes (`push ebp ; lea ebp,[esp-6Ch]` .. `ret 0Ch` at 0x00BB8EEC, int3 padding to
//   0x00BB8EF0 RenderSubPrimitive), no EH frame (the TU is built without /EHsc: the two CArray locals' frees are plain calls).
// Identity: Ego_r.pdb / FableWin.pdb ?RenderLandscapeDecalGroup@CEnginePrimitiveManagerParticleDecalGroup@@AAEXABVCEngineInternalPrimitiveParticleDecalGroup@@_NJ@Z
//   (private, thiscall, three arguments = `ret 0Ch`); FableWin donor order RenderSubPrimitive 0x02EABCA0 .. RenderLandscapeDecalGroup 0x02EAC1D0
//   .. ApplyLandscapeVertexShader 0x02EAD1F0 = retail RenderSubPrimitive 0x00BB8EF0 after this row; its only caller is that RenderSubPrimitive
//   (0x00BB9533, `if (group.LandscapeDecal)`: [group+0C0h] tested at 0x00BB951E).
// Callees (declared by the shared headers, spellings as bound in rebuild/oracles/reloc_bindings.tsv): CShaderRenderManager::ApplyVertexShader
//   0x00988020 (through the inline ApplyLandscapeVertexShader, whose body compiles byte-identically to retail's unreferenced out-of-line copy
//   0x00BB97F0), ApplyPixelShader 0x00988140, SetWorldTransform 0x009881F0, SetUserVertexShaderConstant(unsigned long, const CRGBFloatColour&)
//   0x00989B50, the inline PrepareForRender(0x15) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants
//   0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateWorldSpaceLighting 0x0098A540,
//   UpdateCombinedProjectionTransform 0x00988A50, UpdateCameraPos 0x00989700), CRenderManagerCore::DrawAsIndexedTris 0x00A0AF40,
//   std::vector<CTVertexNormTex1>::vector(size_type) 0x00BB9FD0 (landed; out of line, CArray<CTVertexNormTex1>(n) forwards to it), malloc
//   0x00BFEA0E / free 0x00BFEA14 (the inline vector<unsigned short>(n) and both CArray destructors), __ftol2 0x00BFEA70 ((long) of a float).
// Globals: NEngineInternal::GPShaderRenderManager 0x01436E14, GPRenderManager 0x01436E18, GPCamera 0x01436EA0 (established spellings);
//   float pool 0x0122F59C = 0.5f, 0x0122DED8 = 1.0f, 0x01231724 = 1/255 (CRGBFloatColour(const CRGBColour&)).
// Retail-asm: bare `fistp dword` blocks of the shared helpers -- GFFloatToLongFloor (engine/GFFloatToLongFloor.h) at 0x00BB8A75..0x00BB8A8F
//   (min x) and 0x00BB8AA1..0x00BB8ABB (min y): `fld [ebp+64h] ; fsub [half] ; fistp dword ptr [r] ; fild [r] ; fadd [one] ; fstp [n] ; mov eax,
//   [ebp+64h] ; cmp eax,[n] ; jne ; inc dword ptr [r]`; GFFloatToLongCeil (engine/GFFloatToLongCeil.h) at 0x00BB8ACD..0x00BB8AE7 (max x) and
//   0x00BB8AF9..0x00BB8B13 (max y): `fadd [half]` / `fsub [one]` / `dec dword ptr [r]`; the eight 0.5f / 1.0f frame constants are hoisted out of
//   the decal loop (0x00BB8A20..0x00BB8A51). Per-site attempts (fragment TU probe/asm/spell.cpp, each spelling fed `pos -/+ size * 0.5f` as
//   these sites are, cl 13.10.3077 /O2 and /O2 /QIfist; table probe/asm/result.tsv): Floor sites 30 C++ spellings ((long)floor(x), (long)floorf(x),
//   (long)x with a compare fix-up, (long)(x - 0.5f) with retail's == fix-up written in C++, (int) / (short) / via double, ...), Ceil sites 31 (the
//   ceil mirrors, -(long)floor(-x), ...): every one `call __ftol2` under /O2 and `fistp qword` under /QIfist, never a bare `fistp dword`;
//   12 intrinsic spellings (_mm_cvtss_si32, _mm_cvttss_si32, _mm_cvt_ss2si, _mm_cvtt_ss2si, _mm_cvtsd_si32, _mm_cvttsd_si32, _mm_cvtps_pi32,
//   _mm_cvttps_pi32, _mm_cvtps_epi32, _mm_cvttps_epi32 + two biased forms) emit cvtss2si / cvttss2si / cvtsd2si / cvttsd2si / cvtps2pi /
//   cvttps2pi / cvtps2dq / cvttps2dq. So the shared helpers stay (ASM_RULES step 2); no asm in this row.
// Layout: overlay headers (hdr/README.md): engine/CEnginePrimitiveManagerParticleDecalGroup.h (typed opt-in), engine/CEngineInternalPrimitive-
//   ParticleGroup.h (typed opt-in: Primitives = STLport list<CEnginePrimitiveHandle> +0x48), new engine/CEngineInternalPrimitiveParticleDecalGroup.h,
//   CEngineInternalPrimitiveParticleDecal.h (WorldPos +0x48, Size +0x54, Colour +0x5C, RHSet +0x64), CTVertexNormTex1.h / CTVertexNormTex1Base.h,
//   engine/CMatrix3x4.h (the pending c2005 SetIdentity opt-in). Render states: CRenderStateManager's inline bool setters on DeviceState
//   (+0x2818): AlphaBlendEnable +0xA0, ZBufferEnable +0x50, ZBufferWriteEnable +0x3C, AlphaTestEnable +0xB4, AlphaRef +0xC8.
// Shape: `row = width + 1` is a named local (retail keeps it in ebx after `inc ebx`, 0x00BB8BC7, and spills width to [ebp+64h]); the index
//   writes read row / width as retail's leas do (`row + flip + vi`, `vi - flip + 1`, `row + vi`, `width + vi + 2`); num_verts is computed
//   before v_scale, so v_scale's frame node does not conflict with height and shares its word ([ebp+54h], 0x00BB8BDF) -- with v_scale first,
//   the vertex counter took that word and the five w4 nodes (iterator, num_tris, min x / y, y) sorted differently (frame-packer trace).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#include "fable_compiler.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"
#include "engine/CEngineInternalPrimitiveParticleDecalGroup.h"
#include "engine/CEngineInternalPrimitiveParticleDecal.h"
#include "engine/CTVertexNormTex1.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0

void CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup(const CEngineInternalPrimitiveParticleDecalGroup& group,
                                                                          bool premultiplied_alpha, long point_light_count)
{
    ApplyLandscapeVertexShader(point_light_count);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(LandscapeDecalPixelShader);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    CMatrix3x4 world;
    world.SetIdentity();
    world.SetTranslateOnly(NEngineInternal::GPCamera->PeekCameraPos());
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(world);
    for (std::list<CEnginePrimitiveHandle>::const_iterator it = group.Primitives.begin(); it != group.Primitives.end(); ++it) {
        const CEngineInternalPrimitiveParticleDecal& decal = (const CEngineInternalPrimitiveParticleDecal&)it->GetData();
        // The landscape cells the decal touches.
        long min_x = GFFloatToLongFloor(decal.WorldPos.X - decal.Size.X * 0.5f);
        long min_y = GFFloatToLongFloor(decal.WorldPos.Y - decal.Size.Y * 0.5f);
        long max_x = GFFloatToLongCeil(decal.WorldPos.X + decal.Size.X * 0.5f);
        long max_y = GFFloatToLongCeil(decal.WorldPos.Y + decal.Size.Y * 0.5f);
        long width = max_x - min_x;
        long height = max_y - min_y;
        long num_tris = width * height * 2;
        if (num_tris > 0) {
            CRGBFloatColour colour(decal.Colour);
            if (premultiplied_alpha) {
                colour.R *= colour.A;
                colour.G *= colour.A;
                colour.B *= colour.A;
            }
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, colour);
            float u_scale = 1.0f / decal.Size.X;
            long row = width + 1;
            long num_verts = row * (height + 1);
            float v_scale = 1.0f / decal.Size.Y;
            float u_offset = 0.5f - decal.WorldPos.X * u_scale;
            float v_offset = 0.5f - decal.WorldPos.Y * v_scale;
            CArray<CTVertexNormTex1> vertices(num_verts);
            long v = 0;
            for (long y = min_y; y <= max_y; ++y) {
                for (long x = min_x; x <= max_x; ++x) {
                    vertices[v].X = (float)x;
                    vertices[v].Y = (float)y;
                    vertices[v].Z = decal.WorldPos.Z;
                    vertices[v].NormalX = decal.RHSet.GetUp().X;
                    vertices[v].NormalY = decal.RHSet.GetUp().Y;
                    vertices[v].NormalZ = decal.RHSet.GetUp().Z;
                    vertices[v].U1 = vertices[v].X * u_scale + u_offset;
                    vertices[v].V1 = vertices[v].Y * v_scale + v_offset;
                    ++v;
                }
            }
            // Two triangles per cell; the diagonal alternates with the parity of (x + y), so neighbouring cells mirror each other.
            CArray<unsigned short> indices(num_tris * 3);
            long i = 0;
            long vi = 0;
            for (long t = 0; t < num_tris; t += 2) {
                long flip = 1 - ((long)(vertices[vi].X + vertices[vi].Y) & 1);
                indices[i++] = (unsigned short)vi;
                indices[i++] = (unsigned short)(row + flip + vi);
                indices[i++] = (unsigned short)(vi + 1);
                indices[i++] = (unsigned short)(vi - flip + 1);
                indices[i++] = (unsigned short)(row + vi);
                indices[i++] = (unsigned short)(width + vi + 2);
                vi += 1 + ((vi + 2) % row == 0);   // skip the last vertex of each row
            }
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0x15);
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(vertices.GetAsCArray(), sizeof(CTVertexNormTex1), num_verts,
                                                                 indices.GetAsCArray(), num_tris);
        }
    }
}
