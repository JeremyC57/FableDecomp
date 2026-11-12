// CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector& pos) const -- a weather box is clipped when the landscape under all four corners of its footprint lies at or above the box top (a corner without map data, or one whose landscape is below the top, leaves it visible)
// Grade: DIFFER(4482v4489) -- 1351 of 1528 instructions equal, every difference in corner 4 (0x00B53AF0..0x00B53F59): retail keeps y - MapOffsetY in edi and the grid index in esi (0x00B53B65 `mov edi,[ebp+8]`, 0x00B53BA2 `mov esi,edi`), ours swaps the two; in corner 4's neighbour path retail keeps lx / ly / WorldX / WorldY in edx / esi / ebx / edi, spills the map (0x00B53E04 `mov [ebp-2Ch],eax`) and the neighbour cells ([ebp+8] / [ebp-34h]) and tests `cmp dword ptr [ecx+8],0` (0x00B53EC6); ours spills WorldY and the map instead (7 bytes shorter). Same instruction stream otherwise (same reassociated sums and reloads); corners 1..3 and the frame are byte-identical.
// Tried (7 grades, pass 1; ~45 free probe compiles): g1 default budget (703 bytes), g2 C3DVector half (frame 0x38), g3 = this source
//   (1351 / 1528), g4 function-scope x / y (object identical to g3), g5 this file after the header clean-up (= g3), g6 the "explicit world
//   wrapper" model (CEngineMap::MapHasDataAtPoint as the c2021 && chain, PeekLandscapeHeight's neighbour test through it, the four
//   CEngineWorldMap::MapHasDataAtPoint copies written out with `PeekGameMap() ? true : false`; its COMDATs 0x00B56880 MATCH / 0x00B566B0
//   RELOCATION_MATCH, corner 4 gets retail's edi / `cmp [ecx+8],0`, but corners 1..3 lose: 1139 / 1528, 4523 bytes, /d2inlT507), g7 this
//   model with the neighbour cells unnamed (corner 4's y register right, corner 1's neighbour cells x-first: 1204 / 1528). Objects identical
//   to g3 (free screen, tools/prop.sh): outer k = 1..7 forwarded names, the else-if world wrapper, the `if (out) return false;` map body,
//   C3DVector& span, halfZ local, top first, split ifs, a named height, ternary / if-else tails, FABLE_RETAIL_CARRAY, both wrapper
//   definition orders. Lead: retail's corner 4 behaves as if the neighbour cells were call-argument temporaries (y first) while the
//   world wrapper's test is an if-return value: a spelling giving both at once is the next thing to find.
// Leaf: ?ClipBoxToLandscape@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z
// Flags: /d2inlT497
// Inline-budget: /d2inlT497 -- inside the two FABLE_FORCEINLINE CEngineWorldMap wrappers (forced: c2 never plain-inlines a callee costing
//   over 450, c2.dll 0x1078D248, and MapHasDataAtPoint costs 533) retail expands every CEngineWorldMap::GetMap(x, y) (c1 73, scores 73..146),
//   CEngineMap::MapHasDataAtPoint (c1 45, 51..141) and CEngineMap::PeekLandscapeHeight (c1 338, score 496 at f 0.5); tools/inl.sh trace:
//   T497 is the smallest value that expands all 225 sites (T496 calls the four PeekLandscapeHeight sites). The CEngineMap TU's rows carry
//   T444 / T576 for the same plain-inline bodies (c2021).
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
//   `fld [v] ; fistp dword ptr [r]`). Each helper header carries the proof that no VC7.1 C++ spelling emits a bare dword fistp; the per-site
//   C++ attempts table ASM_RULES asks for was NOT produced for this parked row (debt before landing).
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
