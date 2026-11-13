// CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector& pos) const -- a weather box is clipped when the landscape under all four corners of its footprint lies at or above the box top (a corner without map data, or one whose landscape is below the top, leaves it visible)
// Grade: RELOCATION_MATCH (4489v4489) at /d2inlT507 under the chunk-2037 overlay (g8). The residue of g1..g7 (corner 4,
//   0x00B53B65..0x00B53F13) was the shared header, not this body: CEngineMap::PeekLandscapeHeight's written-out neighbour test passed
//   its named cells to the recursive call; with the call's own argument expressions (engine/CEngineMap.h, IFRETURN opt-in) corner 4
//   spills the cells as retail. The body below is g3's unchanged.
// Leaf: ?ClipBoxToLandscape@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z
// Flags: /d2inlT507
// Inline-budget: /d2inlT507 -- inside the two FABLE_FORCEINLINE CEngineWorldMap wrappers (forced: c2 never plain-inlines a callee costing
//   over 450, c2.dll 0x1078D248, and MapHasDataAtPoint costs 533) retail expands every CEngineWorldMap::GetMap(x, y) (c1 73, scores 73..146),
//   CEngineMap::MapHasDataAtPoint (c1 45, 51..141) and CEngineMap::PeekLandscapeHeight (c1 345, score 506 at f 0.5, four sites);
//   tools/inl.sh trace (tools/inl/xL3.txt): T507 is the smallest value that expands all four PeekLandscapeHeight sites (score 506).
//   c1 345 is the cost c2021 measured for the same body in the CEngineMap TU (0x00B3F5E0, score 443 at f 1, /d2inlT444).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B52DD0, 4489 bytes (0x00B52DD0..0x00B53F59; int3 padding before and after), thiscall, `ret 4`,
//   ebp frame 0x34 + ebx / esi / edi; the dead `pos` home [ebp+8] is reused in corner 4. Callers: RenderRain 0x00B54C65, RenderSnow 0x00B55395.
// Identity (two sources): Ego_r.pdb 0x009F3A70 / FableWin.pdb 0x02D3AF40 ?ClipBoxToLandscape@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z
//   (private const; FableWin lists CEngineWorldMap::PeekLandscapeHeight 0x02D3B1F0 and MapHasDataAtPoint 0x02D3B260 right after it, retail
//   keeps their uncalled COMDATs in this TU at 0x00B566B0 / 0x00B56880). Enumerator names WEATHER_BOX_VISIBLE (0) / WEATHER_BOX_CLIPPED (1)
//   are descriptive (the PDB name sets carry no enumerators of EWeatherBoxVisible).
// Callees: CEngineMap::PeekLandscapeHeight 0x00B439C0 (out-of-line copy, called only from the four expanded neighbour paths:
//   0x00B531DD / 0x00B5362A / 0x00B53A77 / 0x00B53ECE), CIDrawMap::DrawBlockGetSizeZAt (vtable slot 5, `call [edx+14h]`). Global
//   g_pEngineWorldMap 0x01436E8C (established spelling). Constants 0.5f 0x0122F59C, 128.0f 0x012571B0, 1/128 0x012A1F34, 0.0f 0x0122DEDC.
// Shape: the half extents X / Y are two float locals and the box top `pos.Z + Span.Z * 0.5f` a third (a C3DVector half takes a 12-byte frame
//   node: frame 0x38, grade 2); the four corners in retail's layout order (floor X, floor Y), (floor X, ceil Y), (ceil X, floor Y),
//   (ceil X, ceil Y), each `if (!MapHasDataAtPoint(x, y) || PeekLandscapeHeight(x, y) < top) return WEATHER_BOX_VISIBLE;`
//   (`fcomp [top] ; test ah,5 ; jnp` to the shared `xor eax,eax` exit). CEngineWorldMap::MapHasDataAtPoint returns CEngineMap's
//   if-return body as a setne value (0x00B52F7F..0x00B52F87, then 0x00B530D0 `test al,al`); see hdr/README.md for the header conflict.
// Retail-asm: bare `fld ; fistp dword ptr ; fild` blocks of the shared helpers. GFFloatToLongFloor (engine/GFFloatToLongFloor.h) at 0x00B52E22..0x00B52E3C, 0x00B52E56..0x00B52E70, 0x00B5326F..0x00B53289,
//   0x00B536F0..0x00B5370A (`fld [v] ; fsub [half] ; fistp dword ptr [r] ; fild [r] ; fadd [one] ; fstp [n] ; mov eax,[v] ; cmp eax,[n] ;
//   jne ; inc dword ptr [r]`); GFFloatToLongCeil (engine/GFFloatToLongCeil.h) at 0x00B532A3..0x00B532BD, 0x00B536BC..0x00B536D6,
//   0x00B53B09..0x00B53B23, 0x00B53B3D..0x00B53B57 (`fadd [half]` / `fsub [one]` / `dec`); GFFloatToLongNear (QuantiseLandscapeHeight,
//   engine/GFFloatToLongNear.h) at 0x00B53231..0x00B53234, 0x00B5367E..0x00B53681, 0x00B53ACB..0x00B53ACE, 0x00B53F1F..0x00B53F22 (bare
//   `fld [v] ; fistp dword ptr [r]`). Each helper header carries the proof that no VC7.1 C++ spelling emits a bare dword fistp. Per-site attempts
//   (one fragment TU, cl 13.10.3077 /O2 and /O2 /QIfist; table probe/asm/result.tsv, spellings probe/asm/spell.tsv): Near sites 32 C++
//   spellings ((long)x, (int)x, (short)x, (long)(double)x, (long)(x + 0.5f), (long)floor(x + 0.5f), the magic-number add, (__int64)x, ...),
//   Floor sites 30 ((long)floor(x), floorf, (long)x with a compare fix-up, (long)(x - 0.5f) with retail's == fix-up written in C++, ...),
//   Ceil sites 31 (the ceil mirrors, -(long)floor(-x), ...): every one calls __ftol2 under /O2 and emits `fistp qword` under /QIfist,
//   never a bare `fistp dword`; 12 intrinsic spellings (_mm_cvtss_si32, _mm_cvttss_si32, _mm_cvt_ss2si, _mm_cvtt_ss2si, _mm_cvtsd_si32,
//   _mm_cvttsd_si32, _mm_cvtps_pi32, _mm_cvttps_pi32, _mm_cvtps_epi32, _mm_cvttps_epi32 + two biased forms) emit cvtss2si / cvttss2si /
//   cvtsd2si / cvttsd2si / cvtps2pi / cvttps2pi / cvtps2dq / cvttps2dq. So the shared helpers stay (ASM_RULES step 2).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CENGINEWORLDMAP_PEEKLANDSCAPE
#define FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"

CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector& pos) const
{
    float halfX = Settings.ParticleBoxSpan.X * 0.5f;
    float halfY = Settings.ParticleBoxSpan.Y * 0.5f;
    float top = pos.Z + Settings.ParticleBoxSpan.Z * 0.5f;
    {
        long x = GFFloatToLongFloor(pos.X - halfX);
        long y = GFFloatToLongFloor(pos.Y - halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongFloor(pos.X - halfX);
        long y = GFFloatToLongCeil(pos.Y + halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongCeil(pos.X + halfX);
        long y = GFFloatToLongFloor(pos.Y - halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongCeil(pos.X + halfX);
        long y = GFFloatToLongCeil(pos.Y + halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    return WEATHER_BOX_CLIPPED;
}